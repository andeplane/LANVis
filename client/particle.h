#ifndef PARTICLE_H
#define PARTICLE_H
#include <QString>
#include <QVector3D>
#include <QVector4D>

struct ColoredParticle
{
    QVector3D position;
    QVector4D color;
    float radius;
    ColoredParticle() : color(QVector4D(1.0,0.9,0.8, 1.0)), radius(1.0) { }
};

struct IdentifiableParticle
{
    QVector3D position;
    QString type;
    int index;
    IdentifiableParticle() : index(0) { }
    IdentifiableParticle(QVector3D position, QString type, int index) : position(position), type(type), index(index) { }
};

#endif // PARTICLE_H

