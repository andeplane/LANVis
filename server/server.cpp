#include "lammpsbinaryreader.h"
#include "lammpstextdumpreader.h"
#include "server.h"
#include "xyzbinaryreader.h"
#include "xyzreader.h"

#include <cmath>
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QLockFile>
#include <random>

Server::Server() : m_maxNumberOfParticles(300000), m_chunkSize(50), m_lodDistance(250), m_lodLevels(5), m_currentStateIndex(0), m_sort(true), m_currentState(nullptr)
{

}

void Server::loadXYZ(QString fileName)
{
//    XYZReader reader;
//    reader.readFile(fileName);
//    const QVector<QVector3D> &positions = reader.positions();
//    const QVector<QString>   &types     = reader.types();
//    m_allParticles.resize(positions.size());
//    QVector3D min, max;
//    min = positions[0];
//    max = positions[0];

//    for(int particleIndex=0; particleIndex<positions.size(); particleIndex++) {
//        const QVector3D &position = positions.at(particleIndex);
//        float radius = 1.0;
//        QVector3D color(1.0, 0.9, 0.8);

//        if(m_particleStyles.contains(types[particleIndex])) {
//            radius = m_particleStyles[types[particleIndex]]->radius;
//            color[0] = m_particleStyles[types[particleIndex]]->color.redF();
//            color[1] = m_particleStyles[types[particleIndex]]->color.greenF();
//            color[2] = m_particleStyles[types[particleIndex]]->color.blueF();
//        }

//        m_allParticles[particleIndex].color = color;
//        m_allParticles[particleIndex].radius = radius;
//        m_allParticles[particleIndex].position = position;
//        min[0] = std::min(min[0], position[0]);
//        max[0] = std::max(max[0], position[0]);
//        min[1] = std::min(min[1], position[1]);
//        max[1] = std::max(max[1], position[1]);
//        min[2] = std::min(min[2], position[2]);
//        max[2] = std::max(max[2], position[2]);
//    }
//    m_origo = min;
//    m_size = max - min;
//    placeParticlesInChunks();
}

void Server::loadXYZBinary(QString fileName)
{
//    XYZBinaryReader reader;
//    bool success = reader.readFile(fileName);
//    if(!success) return;
//    auto &columns = reader.columns();

//    m_allParticles.resize(reader.numParticles());
//    QVector3D min(columns[1][0], columns[2][0], columns[3][0]); // type x y z ...
//    QVector3D max(columns[1][0], columns[2][0], columns[3][0]);
//    int numKeepParticles = 0;
//    for(size_t particleIndex=0; particleIndex<m_allParticles.size(); particleIndex++) {
//        QVector3D position;
//        int type = int(columns[0][particleIndex]);
//        float x = columns[1][particleIndex];
//        float y = columns[2][particleIndex];
//        float z = columns[3][particleIndex];
//        float occupancy = columns[4][particleIndex];
//        float beta = columns[5][particleIndex];

//        if(z > 40 && z < 80) {
//            float radius = 1.0;
//            QVector3D color(1.0, 0.9, 0.8);
//            position[0] = x;
//            position[1] = y;
//            position[2] = z;

//            QString typeStr = QString("%1").arg(type);
//            if(m_particleStyles.contains(typeStr)) {
//                radius   = m_particleStyles[typeStr]->radius;
//                color[0] = m_particleStyles[typeStr]->color.redF();
//                color[1] = m_particleStyles[typeStr]->color.greenF();
//                color[2] = m_particleStyles[typeStr]->color.blueF();
//            }
//            if(type==4 || type==6) {
//                if(beta>0.5) {
//                    color = QVector3D(0.8,0.8,0.8);
//                    radius *= 1.9;
//                } else {
//                    float occupancyScale = occupancy;
//                    if(occupancyScale>2.0) occupancyScale = 2.0;
//                    if(occupancyScale<0.0) occupancyScale = 0.0;
//                    color[1] *= 0.5*0.5*occupancyScale + 0.5;
//                    color[2] *= 0.5*0.5*occupancyScale + 0.5;
//                }
//            }

//            m_allParticles[numKeepParticles].color = color;
//            m_allParticles[numKeepParticles].radius = radius;
//            m_allParticles[numKeepParticles].position = position;
//            min[0] = std::min(min[0], position[0]);
//            max[0] = std::max(max[0], position[0]);
//            min[1] = std::min(min[1], position[1]);
//            max[1] = std::max(max[1], position[1]);
//            min[2] = std::min(min[2], position[2]);
//            max[2] = std::max(max[2], position[2]);
//            numKeepParticles++;
//        }
//    }
//    m_allParticles.resize(numKeepParticles);
//    qDebug() << "We now have " << numKeepParticles << " particles";
//    m_origo = min;
//    m_size = max - min;
//    placeParticlesInChunks();
}

void Server::loadLAMMPSBinary(QString fileName)
{
    LAMMPSBinaryReader reader;
    reader.readFile(fileName);
    const std::vector<QVector3D> &positions = reader.positions();
    const std::vector<int>       &types     = reader.types();
    qDebug() << "Found " << reader.positions().size() << " particles.";

    State *state = new State();
    m_states.push_back(state);
    m_currentState = state;
    m_currentState->reset();
    m_currentState->addParticles(positions, types, reader.origo(), reader.size());
    m_currentState->placeParticlesInChunks(m_chunkSize, m_lodLevels);
}

void Server::loadLAMMPSTextDump(QString fileName)
{
    LAMMPSTextDumpReader reader;
    reader.readFile(fileName, m_states);
    qDebug() << "Did read " << m_states.size() << " timesteps. Now placing in chunks.";
    for(State *state : m_states) {
        state->placeParticlesInChunks(m_chunkSize, m_lodLevels);
    }
    m_currentState = m_states.front();
}

void Server::writePositions()
{
    QLockFile lockFile(m_lockFileName);
    if(lockFile.tryLock(200)) {
        std::vector<Particle> &particles = m_stateSubset.particles();
        int numBytes = particles.size()*sizeof(Particle);
        if(numBytes > 0) {
            const char *array = reinterpret_cast<const char*>(&particles.front());

            QFile file(m_dataFileName);
            if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
                qDebug() << "Could not open file " << m_dataFileName;
            }
            file.write(array, numBytes);

            file.close();
        }
        lockFile.unlock();
    }
}

void Server::writeState()
{
    if(!m_currentState) return;

    QFile file(m_stateFileName);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QTextStream stream(&file);
        QJsonObject json;

        json["timestamp"] = QJsonValue::fromVariant(QVariant::fromValue<double>(QDateTime::currentDateTime().toMSecsSinceEpoch()));
        json["particleCount"] = QJsonValue::fromVariant(QVariant::fromValue<int>(m_stateSubset.particles().size()));
        json["binaryFileName"] = m_dataFileName;
        json["lockFileName"] = m_lockFileName;
        json["boundingBoxMin"] = QJsonArray({m_stateSubset.boundingBoxMin().x(), m_stateSubset.boundingBoxMin().y(), m_stateSubset.boundingBoxMin().z()});
        json["boundingBoxMax"] = QJsonArray({m_stateSubset.boundingBoxMax().x(), m_stateSubset.boundingBoxMax().y(), m_stateSubset.boundingBoxMax().z()});
        json["numTimesteps"] = QJsonValue::fromVariant(QVariant::fromValue<int>(m_states.size()));

        QJsonDocument saveObject(json);
        stream << saveObject.toJson();
        file.close();
    } else {
        qDebug() << "Could not open file " << m_stateFileName;
    }
}

QString Server::dataFileName() const
{
    return m_dataFileName;
}

void Server::setDataFileName(const QString &dataFileName)
{
    m_dataFileName = dataFileName;
}

QString Server::stateFileName() const
{
    return m_stateFileName;
}

void Server::setStateFileName(const QString &stateFileName)
{
    m_stateFileName = stateFileName;
}

std::vector<Particle> &Server::particles()
{
    return m_stateSubset.particles();
}

QString Server::lockFileName() const
{
    return m_lockFileName;
}

void Server::setLockFileName(const QString &lockFileName)
{
    m_lockFileName = lockFileName;
}

State *Server::currentState() const
{
    return m_currentState;
}

StateSubset &Server::stateSubset()
{
    return m_stateSubset;
}

bool Server::update(QString clientStateFileName)
{
    QFile loadFile(clientStateFileName);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open state file");
        return false;
    }

    QByteArray stateData = loadFile.readAll();

    QJsonParseError error;
    QJsonDocument doc(QJsonDocument::fromJson(stateData, &error));
    if(doc.isNull()) return false;
    if(error.error != QJsonParseError::NoError) { qDebug() << "JSON Error"; return false;}

    QJsonObject   obj = doc.object();
    QJsonArray    arr = obj["cameraPosition"].toArray();
    int maxNumberOfParticles = obj["maxNumberOfParticles"].toInt();
    float chunkSize = obj["chunkSize"].toDouble();
    float lodDistance = obj["lodDistance"].toDouble();
    int lodLevels = obj["lodLevels"].toInt();
    int timestep = obj["timestep"].toInt();
    if(timestep == -1) {
        qDebug() << doc.toJson();
    }
    bool sort = obj["sort"].toBool();

    QVector3D newCameraPositon;
    newCameraPositon[0] = arr[0].toDouble();
    newCameraPositon[1] = arr[1].toDouble();
    newCameraPositon[2] = arr[2].toDouble();
    float distanceToOldPositionSquared = (newCameraPositon - m_cameraPosition).lengthSquared();
    bool chunksDirty = fabs(m_chunkSize-chunkSize)>1.0;
    bool lodDirty = fabs(m_lodDistance-lodDistance)>1.0 || lodLevels != m_lodLevels;

    bool anyChanges = distanceToOldPositionSquared > 5 || maxNumberOfParticles!=m_maxNumberOfParticles || m_sort != sort || chunksDirty || lodDirty || m_currentStateIndex != timestep;
    if(!anyChanges && m_currentState) return false;

    m_maxNumberOfParticles = maxNumberOfParticles;
    m_cameraPosition = newCameraPositon;
    m_sort = sort;
    m_currentStateIndex = timestep;

    if(chunksDirty || lodDirty) {
        m_lodLevels = lodLevels;
        m_lodDistance = lodDistance;
        m_chunkSize = chunkSize;
        for(State *state : m_states) {
            state->placeParticlesInChunks(m_chunkSize, m_lodLevels);
        }
    }

    m_currentStateIndex = std::min(m_currentStateIndex, m_states.size()-1);
    if(m_states.size() > 0 && m_currentStateIndex>=0) {
        m_currentState = m_states[m_currentStateIndex];
    }

    if(m_currentState) {
        m_stateSubset.updatePositions(*m_currentState, m_cameraPosition, m_lodDistance, m_lodLevels, m_maxNumberOfParticles, m_sort);
    }
    return true;
}
