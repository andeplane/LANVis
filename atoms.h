#ifndef ATOMS_H
#define ATOMS_H

#include "atomdata.h"
#include "atomstyle.h"
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
public:
    explicit Atoms(QObject *parent = 0);
    void synchronizeRenderer();

    SphereData* sphereData() const;
    BondData* bondData() const;
    bool sort() const;
    bool dirty() const;
    AtomData &atomData();

signals:
    void sphereDataChanged(SphereData* sphereData);
    void bondDataChanged(BondData* bondData);
    void sortChanged(bool sort);
    void dirtyChanged(bool dirty);

public slots:
    void setSort(bool sort);
    void setDirty(bool dirty);

private:
    void generateSphereData(AtomData &atomData);
    AtomData m_atomData;
    QMap<QString, AtomStyle*> m_atomStyleTypes;
    SphereData* m_sphereData;
    QByteArray m_sphereDataBytes;
    BondData* m_bondData;
    bool m_sort;
    bool m_dirty;
    void setDefaultStyle();
};

#endif // ATOMS_H
