#ifndef GENERALPARTICLE_H
#define GENERALPARTICLE_H

#include <QVector3D>

struct ParticleProperty
{
    QString name;
    QVector values;
};

struct GeneralParticle
{
    QVector3D position;
    QVector3D color;
    int       type;
    float     radius;
    QVector<ParticleProperty> properties;
};

#endif // GENERALPARTICLE_H
