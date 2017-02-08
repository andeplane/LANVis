#include "lammpsbinaryreader.h"
#include "server.h"
#include "../client/xyzreader.h"

#include <cmath>
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QLockFile>
#include <random>

Server::Server() : m_chunkSize(50), m_lodDistance(250), m_lodLevels(5), m_sort(true), m_nx(0), m_ny(0), m_nz(0), m_maxNumberOfAtoms(300000)
{
    setDefaultStyles();
}

void Server::setDefaultStyles() {
    m_atomStyles.insert("1", new AtomStyle(2.27, "#F0C8A0"));
    m_atomStyles.insert("2", new AtomStyle(1.52, "#AA0000"));

    m_atomStyles.insert("hydrogen", new AtomStyle(1.20, "#CCCCCC"));
    m_atomStyles.insert("helium", new AtomStyle(1.40, "#D9FFFF"));
    m_atomStyles.insert("lithium", new AtomStyle(1.82, "#CC80FF"));
    m_atomStyles.insert("beryllium", new AtomStyle(1.53, "#C2FF00"));
    m_atomStyles.insert("boron", new AtomStyle(1.92, "#FFB5B5"));
    m_atomStyles.insert("carbon", new AtomStyle(1.70, "#505050"));
    m_atomStyles.insert("nitrogen", new AtomStyle(1.55, "#3050F8"));
    m_atomStyles.insert("oxygen", new AtomStyle(1.52, "#AA0000"));
    m_atomStyles.insert("fluorine", new AtomStyle(1.35, "#90E050"));
    m_atomStyles.insert("neon", new AtomStyle(1.54, "#3050F8"));
    m_atomStyles.insert("sodium", new AtomStyle(2.27, "#AB5CF2"));
    m_atomStyles.insert("magnesium", new AtomStyle(1.73, "#8AFF00"));
    m_atomStyles.insert("aluminium", new AtomStyle(1.84, "#BFA6A6"));
    m_atomStyles.insert("silicon", new AtomStyle(2.27, "#F0C8A0"));
    m_atomStyles.insert("phosphorus", new AtomStyle(1.80, "#FF8000"));
    m_atomStyles.insert("sulfur", new AtomStyle(1.80, "#FFFF30"));
    m_atomStyles.insert("chlorine", new AtomStyle(1.75, "#1FF01F"));
    m_atomStyles.insert("argon", new AtomStyle(1.88, "#80D1E3"));
    m_atomStyles.insert("potassium", new AtomStyle(2.75, "#8F40D4"));
    m_atomStyles.insert("calcium", new AtomStyle(2.31, "#3DFF00"));

    m_atomStyles.insert("H", new AtomStyle(1.20, "#CCCCCC"));
    m_atomStyles.insert("He", new AtomStyle(1.40, "#D9FFFF"));
    m_atomStyles.insert("Li", new AtomStyle(1.82, "#CC80FF"));
    m_atomStyles.insert("Be", new AtomStyle(1.53, "#C2FF00"));
    m_atomStyles.insert("B", new AtomStyle(1.92, "#FFB5B5"));
    m_atomStyles.insert("C", new AtomStyle(1.70, "#505050"));
    m_atomStyles.insert("N", new AtomStyle(1.55, "#3050F8"));
    m_atomStyles.insert("O", new AtomStyle(1.52, "#AA0000"));
    m_atomStyles.insert("F", new AtomStyle(1.35, "#90E050"));
    m_atomStyles.insert("Ne", new AtomStyle(1.54, "#3050F8"));
    m_atomStyles.insert("Na", new AtomStyle(2.27, "#AB5CF2"));
    m_atomStyles.insert("Mg", new AtomStyle(1.73, "#8AFF00"));
    m_atomStyles.insert("Al", new AtomStyle(1.84, "#BFA6A6"));
    m_atomStyles.insert("Si", new AtomStyle(2.27, "#F0C8A0"));
    m_atomStyles.insert("P", new AtomStyle(1.80, "#FF8000"));
    m_atomStyles.insert("S", new AtomStyle(1.80, "#FFFF30"));
    m_atomStyles.insert("Cl", new AtomStyle(1.75, "#1FF01F"));
    m_atomStyles.insert("Ar", new AtomStyle(1.88, "#80D1E3"));
    m_atomStyles.insert("K", new AtomStyle(2.75, "#8F40D4"));
    m_atomStyles.insert("Ca", new AtomStyle(2.31, "#3DFF00"));
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

        if(m_atomStyles.contains(types[particleIndex])) {
            radius = m_atomStyles[types[particleIndex]]->radius;
            color[0] = m_atomStyles[types[particleIndex]]->color.redF();
            color[1] = m_atomStyles[types[particleIndex]]->color.greenF();
            color[2] = m_atomStyles[types[particleIndex]]->color.blueF();
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

void Server::loadLAMMPSBinary(QString fileName)
{
    LAMMPSBinaryReader reader;
    reader.readFile(fileName);
    const std::vector<QVector3D> &positions = reader.positions();
    const std::vector<int>       &types     = reader.types();
    qDebug() << "Found " << reader.positions().size() << " atoms.";
    m_allParticles.resize(positions.size());

    for(int particleIndex=0; particleIndex<positions.size(); particleIndex++) {
        const QVector3D &position = positions.at(particleIndex);
        float radius = 1.0;
        QVector3D color(1.0, 0.9, 0.8);
        QString typeAsString = QString("%1").arg(types[particleIndex]);
        if(m_atomStyles.contains(typeAsString)) {
            radius = m_atomStyles[typeAsString]->radius;
            color[0] = m_atomStyles[typeAsString]->color.redF();
            color[1] = m_atomStyles[typeAsString]->color.greenF();
            color[2] = m_atomStyles[typeAsString]->color.blueF();
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
    unsigned int atomCount = 0;
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
        if(atomCount > m_maxNumberOfAtoms) break;
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

void Server::writeState()
{
    QFile file(m_stateFileName);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QTextStream stream(&file);
        QJsonObject json;

        json["timestamp"] = QJsonValue::fromVariant(QVariant::fromValue<double>(QDateTime::currentDateTime().toMSecsSinceEpoch()));
        json["particleCount"] = QJsonValue::fromVariant(QVariant::fromValue<int>(m_particles.size()));
        json["binaryFileName"] = m_dataFileName;
        json["lockFileName"] = m_lockFileName;
        json["boundingBoxMin"] = QJsonArray({m_boundingBoxMin[0], m_boundingBoxMin[1], m_boundingBoxMin[2]});
        json["boundingBoxMax"] = QJsonArray({m_boundingBoxMax[0], m_boundingBoxMax[1], m_boundingBoxMax[2]});

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

const std::vector<Particle> &Server::particles() const
{
    return m_particles;
}

const std::vector<Particle> &Server::allParticles() const
{
    return m_allParticles;
}

void Server::sortChunks()
{
    std::sort(m_chunkPtrs.begin(), m_chunkPtrs.end(),
        [&](const Chunk* a, const Chunk* b)
    {
        float da = a->minDistanceTo(m_cameraPosition);
        float db = b->minDistanceTo(m_cameraPosition);
        return da < db;
    });
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
    QFile loadFile(clientStateFileName);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open state file");
        return false;
    }

    QByteArray stateData = loadFile.readAll();

    QJsonParseError error;
    QJsonDocument doc(QJsonDocument::fromJson(stateData, &error));
    if(doc.isNull()) return false;

    QJsonObject   obj = doc.object();

    QJsonArray    arr = obj["cameraPosition"].toArray();
    int maxNumberOfAtoms = obj["maxNumberOfAtoms"].toInt();
    float chunkSize = obj["chunkSize"].toDouble();
    float lodDistance = obj["lodDistance"].toDouble();
    int lodLevels = obj["lodLevels"].toInt();
    bool sort = obj["sort"].toBool();

    QVector3D newCameraPositon;
    newCameraPositon[0] = arr[0].toDouble();
    newCameraPositon[1] = arr[1].toDouble();
    newCameraPositon[2] = arr[2].toDouble();
    float distanceToOldPositionSquared = (newCameraPositon - m_cameraPosition).lengthSquared();
    bool chunksDirty = fabs(m_chunkSize-chunkSize)>1.0;
    bool lodDirty = fabs(m_lodDistance-lodDistance)>1.0 || lodLevels != m_lodLevels;

    bool anyChanges = distanceToOldPositionSquared > 5 || maxNumberOfAtoms!=m_maxNumberOfAtoms || m_sort != sort || chunksDirty || lodDirty;
    if(!anyChanges) return false;

    m_maxNumberOfAtoms = maxNumberOfAtoms;
    m_cameraPosition = newCameraPositon;
    m_sort = sort;

    if(chunksDirty || lodDirty) {
        m_lodLevels = lodLevels;
        m_lodDistance = lodDistance;
        m_chunkSize = chunkSize;
        placeParticleInChunks();
    }

    updatePositions();
    return true;
}
