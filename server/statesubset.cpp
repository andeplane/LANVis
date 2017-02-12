#include "statesubset.h"

StateSubset::StateSubset(QObject *parent) : QObject(parent), m_state(nullptr)
{

}

std::vector<Particle> &StateSubset::particles()
{
    return m_particles;
}

QVector3D StateSubset::boundingBoxMin() const
{
    return m_boundingBoxMin;
}

QVector3D StateSubset::boundingBoxMax() const
{
    return m_boundingBoxMax;
}

void StateSubset::updatePositions(State &state, QVector3D cameraPosition, float lodDistance, int lodLevels, int maxNumberOfParticles, bool sort)
{
    m_state = &state;

    m_particles.clear();
    int particleCount = 0;
    state.sortChunks(cameraPosition);

    m_boundingBoxMin = QVector3D(1e9,1e9,1e9);
    m_boundingBoxMax = QVector3D(-1e9,-1e9,-1e9);

    for(Chunk *chunk : state.chunkPtrs()) {
        float distance = chunk->minDistanceTo(cameraPosition);
        int lod = distance / lodDistance;
        if(lod > lodLevels) lod = lodLevels;

        if(sort) chunk->sort(cameraPosition, lod);
        m_boundingBoxMin[0] = std::min(m_boundingBoxMin[0], chunk->corners()[0][0]);
        m_boundingBoxMin[1] = std::min(m_boundingBoxMin[1], chunk->corners()[0][1]);
        m_boundingBoxMin[2] = std::min(m_boundingBoxMin[2], chunk->corners()[0][2]);

        m_boundingBoxMax[0] = std::max(m_boundingBoxMax[0], chunk->corners()[7][0]);
        m_boundingBoxMax[1] = std::max(m_boundingBoxMax[1], chunk->corners()[7][1]);
        m_boundingBoxMax[2] = std::max(m_boundingBoxMax[2], chunk->corners()[7][2]);

        m_particles.insert( m_particles.end(), chunk->particles(lod).begin(), chunk->particles(lod).end() );
        particleCount += chunk->particles(lod).size();
        if(particleCount > maxNumberOfParticles) break;
    }
}
