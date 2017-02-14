#include "chunk.h"
#include <cmath>

Chunk::Chunk() {
    m_corners.resize(8);
    m_particleIndices.resize(1);
}

void Chunk::reset()
{
    for(size_t i=0; i<m_particleIndices.size(); i++) {
        m_particleIndices[i].clear();
    }
}

float Chunk::minDistanceTo(const QVector3D &point) const
{
    return nearestCorner(point).distanceToPoint(point);
}

QVector3D Chunk::nearestCorner(const QVector3D &point) const
{
    QVector3D nearestCorner = m_corners[0];
    float minDistance = m_corners[0].distanceToPoint(point);

    for(int cornerIndex=1; cornerIndex<8; cornerIndex++) {
        float distance = m_corners[cornerIndex].distanceToPoint(point);
        if(distance < minDistance) {
            minDistance = distance;
            nearestCorner = m_corners[cornerIndex];
        }
    }

    return nearestCorner;
}

void Chunk::sort(const QVector3D &point, const std::vector<IdentifiableParticle> &particles, int lod)
{
    std::sort(m_particleIndices[lod].begin(), m_particleIndices[lod].end(),
        [&](int a, int b)
    {
        float da = (particles[a].position - point).lengthSquared();
        float db = (particles[b].position - point).lengthSquared();
        return da < db;
    });
}

std::vector<int> &Chunk::particleIndices(int lod)
{
    return m_particleIndices[lod];
}

std::vector<QVector3D> &Chunk::corners()
{
    return m_corners;
}

void Chunk::buildLOD(int levels, std::mt19937 &generator, std::uniform_real_distribution<float> &distribution)
{
    m_particleIndices.resize(levels+1);

    for(int lod=1; lod<levels; lod++) {
        float p = pow(0.75, lod);
        int numParticles = m_particleIndices[0].size();

        m_particleIndices[lod].reserve(numParticles*p);
        for(int particleIndex : m_particleIndices[0]) {
            if(distribution(generator) < p) {
                m_particleIndices[lod].push_back(particleIndex);
            }
        }
    }
}
