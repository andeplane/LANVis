#include "chunk.h"

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

void Chunk::sort(const QVector3D &point)
{
    std::sort(m_particles.begin(), m_particles.end(),
        [&](const Particle& a, const Particle& b)
    {
        float da = (a.position - point).lengthSquared();
        float db = (b.position - point).lengthSquared();
        return da < db;
    });
}

std::vector<Particle> &Chunk::particles()
{
    return m_particles;
}

std::vector<QVector3D> &Chunk::corners()
{
    return m_corners;
}
