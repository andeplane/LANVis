#include "particlesubset.h"

ParticleSubset::ParticleSubset(QObject *parent) : QObject(parent), m_state(nullptr)
{

}

std::vector<ColoredParticle> &ParticleSubset::particles()
{
    return m_particles;
}

QVector3D ParticleSubset::boundingBoxMin() const
{
    return m_boundingBoxMin;
}

QVector3D ParticleSubset::boundingBoxMax() const
{
    return m_boundingBoxMax;
}

void ParticleSubset::updatePositions(State &state, const ClientState &clientState)
{
    m_state = &state;

    m_particles.clear();
    int particleCount = 0;
    state.sortChunks(clientState.cameraPosition());

    m_boundingBoxMin = QVector3D(1e9,1e9,1e9);
    m_boundingBoxMax = QVector3D(-1e9,-1e9,-1e9);

    m_particleIndices.clear();
    m_particleIndices.reserve(clientState.maxNumberOfParticles());

    for(Chunk *chunk : state.chunkPtrs()) {
        float distance = chunk->minDistanceTo(clientState.cameraPosition());
        int lod = distance / clientState.lodDistance();
        if(lod > clientState.lodLevels()) lod = clientState.lodLevels();

        if(clientState.sort()) chunk->sort(clientState.cameraPosition(), lod);
        m_boundingBoxMin[0] = std::min(m_boundingBoxMin[0], chunk->corners()[0][0]);
        m_boundingBoxMin[1] = std::min(m_boundingBoxMin[1], chunk->corners()[0][1]);
        m_boundingBoxMin[2] = std::min(m_boundingBoxMin[2], chunk->corners()[0][2]);

        m_boundingBoxMax[0] = std::max(m_boundingBoxMax[0], chunk->corners()[7][0]);
        m_boundingBoxMax[1] = std::max(m_boundingBoxMax[1], chunk->corners()[7][1]);
        m_boundingBoxMax[2] = std::max(m_boundingBoxMax[2], chunk->corners()[7][2]);


        m_particleIndices.insert( m_particleIndices.end(), chunk->particleIndices(lod).begin(), chunk->particleIndices(lod).end() );
        particleCount += chunk->particleIndices(lod).size();
        if(particleCount > clientState.maxNumberOfParticles()) break;
    }
}
