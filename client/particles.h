#ifndef PARTICLES_H
#define PARTICLES_H

#include "particle.h"
#include <QObject>
#include <SimVis/SphereData>
#include <SimVis/BondData>

class Particles : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SphereData* sphereData READ sphereData NOTIFY sphereDataChanged)
    Q_PROPERTY(QVector3D boundingBoxMin READ boundingBoxMin WRITE setBoundingBoxMin NOTIFY boundingBoxMinChanged)
    Q_PROPERTY(QVector3D boundingBoxMax READ boundingBoxMax WRITE setBoundingBoxMax NOTIFY boundingBoxMaxChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(bool dirty READ dirty WRITE setDirty NOTIFY dirtyChanged)
    Q_PROPERTY(float sphereScale READ sphereScale WRITE setSphereScale NOTIFY sphereScaleChanged)

public:
    explicit Particles(QObject *parent = 0);
    void synchronizeRenderer();
    void generateSphereData();

    SphereData* sphereData() const;
    bool dirty() const;
    bool loadBinary(QString fileName);
    void setData(const QVector<QVector3D> &positions, const QVector<QString> &types);
    void setSphereData(SphereData *sphereData);
    float sphereScale() const;
    int count() const;
    QVector3D boundingBoxMin() const;
    QVector3D boundingBoxMax() const;
    ColoredParticle &operator()(int index);
    QVector<ColoredParticle> &get();

signals:
    void sphereDataChanged(SphereData* sphereData);
    void dirtyChanged(bool dirty);
    void sphereScaleChanged(float sphereScale);
    void countChanged(int count);
    void boundingBoxMinChanged(QVector3D boundingBoxMin);
    void boundingBoxMaxChanged(QVector3D boundingBoxMax);

public slots:
    void setDirty(bool dirty);
    void setSphereScale(float sphereScale);
    void setBoundingBoxMin(QVector3D boundingBoxMin);
    void setBoundingBoxMax(QVector3D boundingBoxMax);

private:
    void setDefaultStyle();

    QVector<ColoredParticle> m_particles;
    SphereData* m_sphereData;
    QByteArray m_sphereDataBytes;
    bool m_dirty;
    float m_sphereScale;
    QVector3D m_boundingBoxMin;
    QVector3D m_boundingBoxMax;
};

#endif // PARTICLES_H
