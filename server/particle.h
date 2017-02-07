#ifndef PARTICLE_H
#define PARTICLE_H
#include <QVector3D>

struct Particle
{
    QVector3D position;
    QVector3D color;
    float radius;
    Particle() { radius = 1.0; color = QVector3D(1.0, 0.9, 0.8); }
};

#endif // PARTICLE_H
