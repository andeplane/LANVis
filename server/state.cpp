#include "state.h"
#include <QDebug>

State::State(QObject *parent) : QObject(parent),
    m_nx(0), m_ny(0), m_nz(0), m_chunkSize(0)
{

}

State::State(const State &state)
{
    qDebug() << "Error, should not call this...";
}

State::~State()
{
    m_chunkPtrs.clear();
    m_chunks.clear();
    m_allParticles.clear();
}

const std::vector<Particle> &State::allParticles() const
{
    return m_allParticles;
}

void State::sortChunks(QVector3D point)
{
    std::sort(m_chunkPtrs.begin(), m_chunkPtrs.end(),
              [&](const Chunk* a, const Chunk* b)
    {
        float da = a->minDistanceTo(point);
        float db = b->minDistanceTo(point);
        return da < db;
    });
}

QVector3D State::boundingBoxMin() const
{
    return m_boundingBoxMin;
}

QVector3D State::boundingBoxMax() const
{
    return m_boundingBoxMax;
}

void State::setBoundingBoxMax(const QVector3D &boundingBoxMax)
{
    m_boundingBoxMax = boundingBoxMax;
}

std::vector<Chunk *> State::chunkPtrs() const
{
    return m_chunkPtrs;
}

void State::setBoundingBoxMin(const QVector3D &boundingBoxMin)
{
    m_boundingBoxMin = boundingBoxMin;
}

void State::placeParticlesInChunks(float chunkSize, int lodLevels)
{
    m_chunkSize = chunkSize;
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
    qDebug() << "Building LOD with " << lodLevels << " levels";
    for(Chunk &chunk : m_chunks) {
        chunk.buildLOD(lodLevels, generator, distribution);
    }
}

void State::setupChunks()
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

