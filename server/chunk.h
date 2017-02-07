#ifndef CHUNK_H
#define CHUNK_H

#include "../client/particle.h"
#include <QVector>
#include <vector>
class Chunk
{
public:
    Chunk() { m_corners.resize(8); }
//    Chunk(Chunk const &) = delete;
//    void operator=(Chunk const &) = delete;
    // ~Chunk() { m_corners.clear(); m_particles.clear(); }

    float minDistanceTo(const QVector3D &point) const;
    QVector3D nearestCorner(const QVector3D &point) const;
    void sort(const QVector3D &point);
    std::vector<QVector3D> &corners();
    std::vector<Particle> &particles();

private:
    std::vector<QVector3D> m_corners;
    std::vector<Particle>  m_particles;
};

#endif // CHUNK_H
