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

Server::Server() :
{
    setDefaultStyles();
}

void Server::setDefaultStyles() {
    m_particleStyles.insert("1", new ParticleStyle(2.27, "#F0C8A0"));
    m_particleStyles.insert("2", new ParticleStyle(1.52, "#AA0000"));
    m_particleStyles.insert("4", new ParticleStyle(1.84, "#119900"));
    m_particleStyles.insert("6", new ParticleStyle(1.84, "#119900"));

    m_particleStyles.insert("5", new ParticleStyle(1.70, "#505050"));
    m_particleStyles.insert("7", new ParticleStyle(1.52, "#AA0000"));
    m_particleStyles.insert("8", new ParticleStyle(3.5, "#F0C8A0"));

    m_particleStyles.insert("hydrogen", new ParticleStyle(1.20, "#CCCCCC"));
    m_particleStyles.insert("helium", new ParticleStyle(1.40, "#D9FFFF"));
    m_particleStyles.insert("lithium", new ParticleStyle(1.82, "#CC80FF"));
    m_particleStyles.insert("beryllium", new ParticleStyle(1.53, "#C2FF00"));
    m_particleStyles.insert("boron", new ParticleStyle(1.92, "#FFB5B5"));
    m_particleStyles.insert("carbon", new ParticleStyle(1.70, "#505050"));
    m_particleStyles.insert("nitrogen", new ParticleStyle(1.55, "#3050F8"));
    m_particleStyles.insert("oxygen", new ParticleStyle(1.52, "#AA0000"));
    m_particleStyles.insert("fluorine", new ParticleStyle(1.35, "#90E050"));
    m_particleStyles.insert("neon", new ParticleStyle(1.54, "#3050F8"));
    m_particleStyles.insert("sodium", new ParticleStyle(2.27, "#AB5CF2"));
    m_particleStyles.insert("magnesium", new ParticleStyle(1.73, "#8AFF00"));
    m_particleStyles.insert("aluminium", new ParticleStyle(1.84, "#BFA6A6"));
    m_particleStyles.insert("silicon", new ParticleStyle(2.27, "#F0C8A0"));
    m_particleStyles.insert("phosphorus", new ParticleStyle(1.80, "#FF8000"));
    m_particleStyles.insert("sulfur", new ParticleStyle(1.80, "#FFFF30"));
    m_particleStyles.insert("chlorine", new ParticleStyle(1.75, "#1FF01F"));
    m_particleStyles.insert("argon", new ParticleStyle(1.88, "#80D1E3"));
    m_particleStyles.insert("potassium", new ParticleStyle(2.75, "#8F40D4"));
    m_particleStyles.insert("calcium", new ParticleStyle(2.31, "#3DFF00"));

    m_particleStyles.insert("H", new ParticleStyle(1.20, "#CCCCCC"));
    m_particleStyles.insert("He", new ParticleStyle(1.40, "#D9FFFF"));
    m_particleStyles.insert("Li", new ParticleStyle(1.82, "#CC80FF"));
    m_particleStyles.insert("Be", new ParticleStyle(1.53, "#C2FF00"));
    m_particleStyles.insert("B", new ParticleStyle(1.92, "#FFB5B5"));
    m_particleStyles.insert("C", new ParticleStyle(1.70, "#505050"));
    m_particleStyles.insert("N", new ParticleStyle(1.55, "#3050F8"));
    m_particleStyles.insert("O", new ParticleStyle(1.52, "#AA0000"));
    m_particleStyles.insert("F", new ParticleStyle(1.35, "#90E050"));
    m_particleStyles.insert("Ne", new ParticleStyle(1.54, "#3050F8"));
    m_particleStyles.insert("Na", new ParticleStyle(2.27, "#AB5CF2"));
    m_particleStyles.insert("Mg", new ParticleStyle(1.73, "#8AFF00"));
    m_particleStyles.insert("Al", new ParticleStyle(1.84, "#BFA6A6"));
    m_particleStyles.insert("Si", new ParticleStyle(2.27, "#F0C8A0"));
    m_particleStyles.insert("P", new ParticleStyle(1.80, "#FF8000"));
    m_particleStyles.insert("S", new ParticleStyle(1.80, "#FFFF30"));
    m_particleStyles.insert("Cl", new ParticleStyle(1.75, "#1FF01F"));
    m_particleStyles.insert("Ar", new ParticleStyle(1.88, "#80D1E3"));
    m_particleStyles.insert("K", new ParticleStyle(2.75, "#8F40D4"));
    m_particleStyles.insert("Ca", new ParticleStyle(2.31, "#3DFF00"));
}

void Server::setupChunks()
{
    m_nx = m_size[0]/m_chunkSize + 1;
    m_ny = m_size[1]/m_chunkSize + 1;
    m_nz = m_size[2]/m_chunkSize + 1;
    int numChunks = m_nx*m_ny*m_nz;
    m_chunks.resize(numChunks);
    m_chunkPtrs.resize(numChunks);
    for(int i=0; i<numChunks; i++) {
        m_chunkPtrs[i] = &m_chunks[i]; // TODO: handle this better than raw pointers
    }

    for(int i=0; i<m_nx; i++) {
        for(int j=0; j<m_ny; j++) {
            for(int k=0; k<m_nz; k++) {
                Chunk &chunk = m_chunks[index(i,j,k)];
                std::vector<QVector3D> &corners = chunk.corners();
                corners[0] = QVector3D(m_origo[0] + (i+0)*m_chunkSize, m_origo[1] + (j+0)*m_chunkSize, m_origo[2] + (k+0)*m_chunkSize);
                corners[1] = QVector3D(m_origo[0] + (i+0)*m_chunkSize, m_origo[1] + (j+0)*m_chunkSize, m_origo[2] + (k+1)*m_chunkSize);
                corners[2] = QVector3D(m_origo[0] + (i+0)*m_chunkSize, m_origo[1] + (j+1)*m_chunkSize, m_origo[2] + (k+0)*m_chunkSize);
                corners[3] = QVector3D(m_origo[0] + (i+1)*m_chunkSize, m_origo[1] + (j+0)*m_chunkSize, m_origo[2] + (k+0)*m_chunkSize);
                corners[4] = QVector3D(m_origo[0] + (i+0)*m_chunkSize, m_origo[1] + (j+1)*m_chunkSize, m_origo[2] + (k+1)*m_chunkSize);
                corners[5] = QVector3D(m_origo[0] + (i+1)*m_chunkSize, m_origo[1] + (j+0)*m_chunkSize, m_origo[2] + (k+1)*m_chunkSize);
                corners[6] = QVector3D(m_origo[0] + (i+1)*m_chunkSize, m_origo[1] + (j+1)*m_chunkSize, m_origo[2] + (k+0)*m_chunkSize);
                corners[7] = QVector3D(m_origo[0] + (i+1)*m_chunkSize, m_origo[1] + (j+1)*m_chunkSize, m_origo[2] + (k+1)*m_chunkSize);
            }
        }
    }
}

void Server::loadXYZ(QString fileName)
{
    XYZReader reader;
    reader.readFile(fileName);
    const QVector<QVector3D> &positions = reader.positions();
    const QVector<QString>   &types     = reader.types();
    m_allParticles.resize(positions.size());
    QVector3D min, max;
    min = positions[0];
    max = positions[0];

    for(int particleIndex=0; particleIndex<positions.size(); particleIndex++) {
        const QVector3D &position = positions.at(particleIndex);
        float radius = 1.0;
        QVector3D color(1.0, 0.9, 0.8);

        if(m_particleStyles.contains(types[particleIndex])) {
            radius = m_particleStyles[types[particleIndex]]->radius;
            color[0] = m_particleStyles[types[particleIndex]]->color.redF();
            color[1] = m_particleStyles[types[particleIndex]]->color.greenF();
            color[2] = m_particleStyles[types[particleIndex]]->color.blueF();
        }

        m_allParticles[particleIndex].color = color;
        m_allParticles[particleIndex].radius = radius;
        m_allParticles[particleIndex].position = position;
        min[0] = std::min(min[0], position[0]);
        max[0] = std::max(max[0], position[0]);
        min[1] = std::min(min[1], position[1]);
        max[1] = std::max(max[1], position[1]);
        min[2] = std::min(min[2], position[2]);
        max[2] = std::max(max[2], position[2]);
    }
    m_origo = min;
    m_size = max - min;
    placeParticleInChunks();
}

void Server::loadXYZBinary(QString fileName)
{
    XYZBinaryReader reader;
    bool success = reader.readFile(fileName);
    if(!success) return;
    auto &columns = reader.columns();

    m_allParticles.resize(reader.numParticles());
    QVector3D min(columns[1][0], columns[2][0], columns[3][0]); // type x y z ...
    QVector3D max(columns[1][0], columns[2][0], columns[3][0]);
    int numKeepParticles = 0;
    for(size_t particleIndex=0; particleIndex<m_allParticles.size(); particleIndex++) {
        QVector3D position;
        int type = int(columns[0][particleIndex]);
        float x = columns[1][particleIndex];
        float y = columns[2][particleIndex];
        float z = columns[3][particleIndex];
        float occupancy = columns[4][particleIndex];
        float beta = columns[5][particleIndex];

        if(z > 40 && z < 80) {
            float radius = 1.0;
            QVector3D color(1.0, 0.9, 0.8);
            position[0] = x;
            position[1] = y;
            position[2] = z;

            QString typeStr = QString("%1").arg(type);
            if(m_particleStyles.contains(typeStr)) {
                radius   = m_particleStyles[typeStr]->radius;
                color[0] = m_particleStyles[typeStr]->color.redF();
                color[1] = m_particleStyles[typeStr]->color.greenF();
                color[2] = m_particleStyles[typeStr]->color.blueF();
            }
            if(type==4 || type==6) {
                if(beta>0.5) {
                    color = QVector3D(0.8,0.8,0.8);
                    radius *= 1.9;
                } else {
                    float occupancyScale = occupancy;
                    if(occupancyScale>2.0) occupancyScale = 2.0;
                    if(occupancyScale<0.0) occupancyScale = 0.0;
                    color[1] *= 0.5*0.5*occupancyScale + 0.5;
                    color[2] *= 0.5*0.5*occupancyScale + 0.5;
                }
            }

            m_allParticles[numKeepParticles].color = color;
            m_allParticles[numKeepParticles].radius = radius;
            m_allParticles[numKeepParticles].position = position;
            min[0] = std::min(min[0], position[0]);
            max[0] = std::max(max[0], position[0]);
            min[1] = std::min(min[1], position[1]);
            max[1] = std::max(max[1], position[1]);
            min[2] = std::min(min[2], position[2]);
            max[2] = std::max(max[2], position[2]);
            numKeepParticles++;
        }
    }
    m_allParticles.resize(numKeepParticles);
    qDebug() << "We now have " << numKeepParticles << " particles";
    m_origo = min;
    m_size = max - min;
    placeParticleInChunks();
}

void Server::loadLAMMPSBinary(QString fileName)
{
    LAMMPSBinaryReader reader;
    reader.readFile(fileName);
    const std::vector<QVector3D> &positions = reader.positions();
    const std::vector<int>       &types     = reader.types();
    qDebug() << "Found " << reader.positions().size() << " particles.";
    m_allParticles.resize(positions.size());

    for(size_t particleIndex=0; particleIndex<positions.size(); particleIndex++) {
        const QVector3D &position = positions.at(particleIndex);
        float radius = 1.0;
        QVector3D color(1.0, 0.9, 0.8);
        QString typeAsString = QString("%1").arg(types[particleIndex]);
        if(m_particleStyles.contains(typeAsString)) {
            radius = m_particleStyles[typeAsString]->radius;
            color[0] = m_particleStyles[typeAsString]->color.redF();
            color[1] = m_particleStyles[typeAsString]->color.greenF();
            color[2] = m_particleStyles[typeAsString]->color.blueF();
        }

        m_allParticles[particleIndex].color = color;
        m_allParticles[particleIndex].radius = radius;
        m_allParticles[particleIndex].position = position;
    }
    m_origo = reader.origo();
    m_size = reader.size();
    placeParticleInChunks();
}

void Server::placeParticleInChunks() {
    setupChunks();
    float oneOverChunkSize = 1.0/m_chunkSize;
    for(Chunk &chunk : m_chunks) {
        chunk.clear();
    }

    for(const Particle &particle : m_allParticles) {
        int i = (particle.position[0]-m_origo[0]) * oneOverChunkSize;
        int j = (particle.position[1]-m_origo[1]) * oneOverChunkSize;
        int k = (particle.position[2]-m_origo[2]) * oneOverChunkSize;
        Chunk &chunk = m_chunks[index(i,j,k)];
        chunk.particles(0).push_back(particle);
    }

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<float> distribution(0, 1);
    qDebug() << "Building LOD with " << m_lodLevels << " levels";
    for(Chunk &chunk : m_chunks) {
        chunk.buildLOD(m_lodLevels, generator, distribution);
    }
}

void Server::updatePositions()
{
    m_particles.clear();
    int atomCount = 0;
    sortChunks();

    m_boundingBoxMin = QVector3D(1e9,1e9,1e9);
    m_boundingBoxMax = QVector3D(-1e9,-1e9,-1e9);

    for(Chunk *chunk : m_chunkPtrs) {
        float distance = chunk->minDistanceTo(m_cameraPosition);
        int lod = distance / m_lodDistance;
        if(lod > 3) lod = 3;

        if(m_sort) chunk->sort(m_cameraPosition, lod);
        m_boundingBoxMin[0] = std::min(m_boundingBoxMin[0], chunk->corners()[0][0]);
        m_boundingBoxMin[1] = std::min(m_boundingBoxMin[1], chunk->corners()[0][1]);
        m_boundingBoxMin[2] = std::min(m_boundingBoxMin[2], chunk->corners()[0][2]);

        m_boundingBoxMax[0] = std::max(m_boundingBoxMax[0], chunk->corners()[7][0]);
        m_boundingBoxMax[1] = std::max(m_boundingBoxMax[1], chunk->corners()[7][1]);
        m_boundingBoxMax[2] = std::max(m_boundingBoxMax[2], chunk->corners()[7][2]);

        m_particles.insert( m_particles.end(), chunk->particles(lod).begin(), chunk->particles(lod).end() );
        atomCount += chunk->particles(lod).size();
        if(atomCount > m_maxNumberOfParticles) break;
    }
}

void Server::writePositions()
{
    QLockFile lockFile(m_lockFileName);
    if(lockFile.tryLock(200)) {
        int numBytes = m_particles.size()*sizeof(Particle);
        if(numBytes > 0) {
            const char *array = reinterpret_cast<const char*>(&m_particles.front());

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

void Server::writeCurrentState()
{

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

QString Server::lockFileName() const
{
    return m_lockFileName;
}

void Server::setLockFileName(const QString &lockFileName)
{
    m_lockFileName = lockFileName;
}

bool Server::update(QString clientStateFileName)
{
    m_clientState.load(clientStateFileName);
    if(m_clientState.chunksDirty()) {
        // placeParticleInChunks();
    }
    if(m_clientState.particlesDirty()) {
        // updatePositions();
    }
}
