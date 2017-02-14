#include "lammpsbinaryreader.h"
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
#include <QElapsedTimer>

Server::Server() : m_lockFileName("/projects/tmp/lanvis.lock"), m_currentState(nullptr)
{
    m_clientState.setFileName("/projects/tmp/client.json");
}

bool Server::loadXYZ(QString fileName)
{
    XYZReader reader;
    bool success = reader.readFile(fileName);
    if(!success) return false;
    const std::vector<QVector3D> &positions = reader.positions();
    const std::vector<QString>   &types     = reader.types();

    qDebug() << "Creating new state";
    m_states.clear();
    State *state = new State();
    m_states.push_back(state);
    qDebug() << "Adding particles";
    state->addParticles(positions, types, reader.origo(), reader.size());
    qDebug() << "Placing particles in chunks";
    state->placeParticlesInChunks(m_clientState);
    m_currentState = state;

    return true;
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
//    placeParticleInChunks();
}

void Server::loadLAMMPSBinary(QString fileName)
{
//    LAMMPSBinaryReader reader;
//    reader.readFile(fileName);
//    const std::vector<QVector3D> &positions = reader.positions();
//    const std::vector<int>       &types     = reader.types();
//    qDebug() << "Found " << reader.positions().size() << " particles.";

//    State *state = new State();
//    m_states.push_back(state);
//    m_currentState = state;
//    m_currentState->reset();
//    m_currentState->addParticles(positions, types, reader.origo(), reader.size());
//    m_currentState->placeParticlesInChunks(m_chunkSize, m_lodLevels);
}

void Server::loadLAMMPSTextDump(QString fileName)
{
//    LAMMPSTextDumpReader reader;
//    reader.readFile(fileName, m_states);
//    qDebug() << "Did read " << m_states.size() << " timesteps. Now placing in chunks.";
//    for(State *state : m_states) {
//        state->placeParticlesInChunks(m_chunkSize, m_lodLevels);
//    }
//    m_currentState = m_states.front();
}

bool Server::loadFile()
{
    if(m_clientState.serverSettings()->inputFileType()=="xyz") {
        qDebug() << "File type is XYZ";
        bool success = loadXYZ(m_clientState.serverSettings()->inputFile()); //TODO: rename to fileName
        return success;
    }
    return false;
}

void Server::writePositions()
{
    QLockFile lockFile(m_lockFileName);
    if(lockFile.tryLock(200)) {
        std::vector<ColoredParticle> &particles = m_subset.particles();
        int numBytes = particles.size()*sizeof(ColoredParticle);
        if(numBytes > 0) {
            const char *array = reinterpret_cast<const char*>(&particles.front());
            QFile file("/projects/tmp/state.bin");
            if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
                qDebug() << "Could not open file state.bin";
            }
            file.write(array, numBytes);

            file.close();
        }
        lockFile.unlock();
    }
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

const ParticleSubset &Server::subset() const
{
    return m_subset;
}

bool Server::update()
{
    bool clientFileExists = m_clientState.load();
    if(!clientFileExists) return false;
    QElapsedTimer t;
    t.start();

    if(m_settings.inputFile()!=m_clientState.serverSettings()->inputFile() || m_settings.inputFileType()!=m_clientState.serverSettings()->inputFileType()) {
        qDebug() << "New input file or input file type, loading file.";
        bool success = loadFile();
        if(success) {
            m_settings.setInputFile(m_clientState.serverSettings()->inputFile());
            m_settings.setInputFileType(m_clientState.serverSettings()->inputFileType());
            qDebug() << "Loading " << m_settings.inputFile() << " took " << t.restart() << " ms.";
        }
    }

    if(!m_currentState) {
        return false;
    }

    if(m_clientState.chunksDirty()) {
        // placeParticleInChunks();
        qDebug() << "Placing particles in chunks took " << t.restart() << " ms.";
        return true;
    }

    if(m_clientState.particlesDirty()) {
        State &state = *m_currentState;
        m_subset.updatePositions(state, m_clientState);
        qDebug() << "Updating positions processing " << state.allParticles().size() << " particles took " << t.restart() << " ms.";
        writePositions();
        save();
        qDebug() << "Writing " << m_subset.particles().size() << " particles to file took " << t.restart() << " ms.";

        m_clientState.setParticlesDirty(false);
        return true;
    }

    return true;
}

void Server::save()
{
    QFile file("/projects/tmp/server.json");
    if(file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QTextStream stream(&file);
        QJsonObject json;

        json["timestamp"] = QJsonValue::fromVariant(QVariant::fromValue<double>(QDateTime::currentDateTime().toMSecsSinceEpoch()));
        json["particleCount"] = QJsonValue::fromVariant(QVariant::fromValue<int>(m_subset.particles().size()));
        json["binaryFileName"] = "/projects/tmp/state.bin";
        json["lockFileName"] = m_lockFileName;
        json["boundingBoxMin"] = QJsonArray({m_subset.boundingBoxMin().x(), m_subset.boundingBoxMin().y(), m_subset.boundingBoxMin().z()});
        json["boundingBoxMax"] = QJsonArray({m_subset.boundingBoxMax().x(), m_subset.boundingBoxMax().y(), m_subset.boundingBoxMax().z()});
        json["origo"] = QJsonArray({m_currentState->origo().x(), m_currentState->origo().y(), m_currentState->origo().z()});
        json["size"] = QJsonArray({m_currentState->size().x(), m_currentState->size().y(), m_currentState->size().z()});
        json["numTimesteps"] = QJsonValue::fromVariant(QVariant::fromValue<int>(m_states.size()));

        QJsonDocument saveObject(json);
        stream << saveObject.toJson();
        file.close();
    } else {
        qDebug() << "Could not open file " << file.fileName();
    }
}
