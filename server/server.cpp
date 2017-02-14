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

Server::Server() : m_currentState(nullptr)
{
    m_clientState.setFileName("/projects/tmp/client.json");
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
//    placeParticleInChunks();
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

void Server::writePositions()
{
    QLockFile lockFile(m_lockFileName);
    if(lockFile.tryLock(200)) {
        std::vector<ColoredParticle> &particles = m_subset.particles();
        int numBytes = particles.size()*sizeof(ColoredParticle);
        if(numBytes > 0) {
            const char *array = reinterpret_cast<const char*>(&particles.front());
            QFile file("state.bin");
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

bool Server::update()
{
    m_clientState.load();

    if(m_clientState.chunksDirty()) {
        // placeParticleInChunks();
    }
    if(m_clientState.particlesDirty()) {
        State &state = *m_currentState;
        m_subset.updatePositions(state, m_clientState);
    }
}
