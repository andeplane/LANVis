#ifndef CHUNK_H
#define CHUNK_H

#include "../client/particle.h"
#include <QVector>

struct Chunk
{
    Chunk() { corners.resize(8); }
    float minDistanceTo(const QVector3D &point);
    QVector<QVector3D> corners;
    QVector<Particle> particles;
};

#endif // CHUNK_H
