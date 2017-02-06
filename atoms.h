#ifndef ATOMS_H
#define ATOMS_H

#include "atomdata.h"
#include <QObject>
#include <SimVis/SphereData>
#include <SimVis/BondData>

class Atoms : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SphereData* sphereData READ sphereData NOTIFY sphereDataChanged)
    Q_PROPERTY(BondData* bondData READ bondData NOTIFY bondDataChanged)
    Q_PROPERTY(bool sort READ sort WRITE setSort NOTIFY sortChanged)

public:
    explicit Atoms(QObject *parent = 0);
    void synchronizeRenderer();

    SphereData* sphereData() const;
    BondData* bondData() const;
    bool sort() const;

signals:
    void sphereDataChanged(SphereData* sphereData);
    void bondDataChanged(BondData* bondData);
    void sortChanged(bool sort);

public slots:
    void setSort(bool sort);

private:
    void generateSphereData(AtomData &atomData);

    AtomData m_atomData;
    SphereData* m_sphereData;
    QByteArray m_sphereDataBytes;
    BondData* m_bondData;
    bool m_sort;
};

#endif // ATOMS_H
