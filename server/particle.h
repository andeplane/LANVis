#ifndef PARTICLE_H
#define PARTICLE_H
#include <QVector3D>

struct Particle
{
    QVector3D position;
    float radius;
    Particle() { radius = 1.0; }
};

#endif // PARTICLE_H
