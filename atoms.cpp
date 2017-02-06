#include "atoms.h"

Atoms::Atoms(QObject *parent) : QObject(parent),
    m_sphereData(nullptr), m_bondData(nullptr), m_sort(false)
{

}

void Atoms::generateSphereData(AtomData &atomData) {

    int visibleAtomCount = 0;
    for(int i = 0; i<atomData.size(); i++) {
        if(atomData.visible[i]) {
            atomData.positions[visibleAtomCount] = atomData.positions[i];
            atomData.colors[visibleAtomCount] = atomData.colors[i];
            atomData.radii[visibleAtomCount] = atomData.radii[i];
            visibleAtomCount++;
        }
    }
    atomData.positions.resize(visibleAtomCount);
    atomData.colors.resize(visibleAtomCount);
    atomData.radii.resize(visibleAtomCount);

    m_sphereDataBytes.resize(visibleAtomCount * sizeof(SphereVBOData));
    SphereVBOData *vboData = reinterpret_cast<SphereVBOData *>(m_sphereDataBytes.data());
    for(int i=0; i<visibleAtomCount; i++) {
        SphereVBOData &vbo = vboData[i];
        vbo.position = atomData.positions[i];
        vbo.color = atomData.colors[i];
        vbo.radius = atomData.radii[i];
    }
}

void Atoms::synchronizeRenderer() {
    int numSpheres = m_sphereDataBytes.size() / sizeof(SphereVBOData);
    m_sphereData->setData(m_sphereDataBytes, numSpheres);
}

SphereData *Atoms::sphereData() const
{
    return m_sphereData;
}

BondData *Atoms::bondData() const
{
    return m_bondData;
}

bool Atoms::sort() const
{
    return m_sort;
}

void Atoms::setSort(bool sort)
{
    if (m_sort == sort)
        return;

    m_sort = sort;
    emit sortChanged(sort);
}
