#ifndef CHUNK_H
#define CHUNK_H

#include "../client/particle.h"
#include <QVector>
#include <vector>
struct Chunk
{
    Chunk() { corners.resize(8); }
    float minDistanceTo(const QVector3D &point) const;
    std::vector<QVector3D> corners;
    std::vector<Particle> particles;
};

#endif // CHUNK_H
