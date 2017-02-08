#include "chunk.h"
#include <cmath>

Chunk::Chunk() {
    m_corners.resize(8);
    m_particles.resize(4); // 4 leves of LOD
}

void Chunk::clear()
{
    for(int i=0; i<m_particles.size(); i++) {
        m_particles[i].clear();
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

void Chunk::sort(const QVector3D &point, int lod)
{
    std::sort(m_particles[lod].begin(), m_particles[lod].end(),
        [&](const Particle& a, const Particle& b)
    {
        float da = (a.position - point).lengthSquared();
        float db = (b.position - point).lengthSquared();
        return da < db;
    });
}

std::vector<Particle> &Chunk::particles(int lod)
{
    return m_particles[lod];
}

std::vector<QVector3D> &Chunk::corners()
{
    return m_corners;
}

void Chunk::buildLOD(int levels, std::mt19937 &generator, std::uniform_real_distribution<float> &distribution)
{
    m_particles.resize(levels+1);

    for(int i=1; i<m_particles.size(); i++) {
        int numParticles = m_particles[0].size() * pow(0.75, i);
        m_particles[i].reserve(numParticles);
    }

    for(const Particle &particle : m_particles[0]) {
        for(int lod=1; lod<levels; lod++) {
            float p = pow(0.75, lod);
            if(distribution(generator) < p) {
                m_particles[lod].push_back(particle);
            }
        }
    }
}
