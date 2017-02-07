#ifndef ATOMS_H
#define ATOMS_H

#include "atomdata.h"
#include "atomstyle.h"
#include "particle.h"
#include <QObject>
#include <SimVis/SphereData>
#include <SimVis/BondData>

class Atoms : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SphereData* sphereData READ sphereData NOTIFY sphereDataChanged)
    Q_PROPERTY(BondData* bondData READ bondData NOTIFY bondDataChanged)
    Q_PROPERTY(bool sort READ sort WRITE setSort NOTIFY sortChanged)
    Q_PROPERTY(bool dirty READ dirty WRITE setDirty NOTIFY dirtyChanged)
    Q_PROPERTY(float sphereScale READ sphereScale WRITE setSphereScale NOTIFY sphereScaleChanged)
public:
    explicit Atoms(QObject *parent = 0);
    void synchronizeRenderer();
    void generateSphereData();

    SphereData* sphereData() const;
    BondData* bondData() const;
    bool sort() const;
    bool dirty() const;

    void loadBinary(QString fileName);
    void setData(const QVector<QVector3D> &positions, const QVector<QString> &types);
    void setSphereData(SphereData *sphereData);
    float sphereScale() const;
signals:
    void sphereDataChanged(SphereData* sphereData);
    void bondDataChanged(BondData* bondData);
    void sortChanged(bool sort);
    void dirtyChanged(bool dirty);
    void sphereScaleChanged(float sphereScale);

public slots:
    void setSort(bool sort);
    void setDirty(bool dirty);
    void setSphereScale(float sphereScale);

private:
    void setDefaultStyle();

    QVector<Particle> m_particles;
    QMap<QString, AtomStyle*> m_atomStyles;
    SphereData* m_sphereData;
    QByteArray m_sphereDataBytes;
    BondData* m_bondData;
    bool m_sort;
    bool m_dirty;
    float m_sphereScale;
};

#endif // ATOMS_H
