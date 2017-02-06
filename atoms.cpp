#include "atoms.h"

Atoms::Atoms(QObject *parent) : QObject(parent),
    m_sphereData(nullptr), m_bondData(nullptr), m_sort(false), m_dirty(false)
{
    setDefaultStyle();
}

void Atoms::setDefaultStyle() {
    m_atomStyleTypes.insert("hydrogen", new AtomStyle(1.20, "#CCCCCC"));
    m_atomStyleTypes.insert("helium", new AtomStyle(1.40, "#D9FFFF"));
    m_atomStyleTypes.insert("lithium", new AtomStyle(1.82, "#CC80FF"));
    m_atomStyleTypes.insert("beryllium", new AtomStyle(1.53, "#C2FF00"));
    m_atomStyleTypes.insert("boron", new AtomStyle(1.92, "#FFB5B5"));
    m_atomStyleTypes.insert("carbon", new AtomStyle(1.70, "#505050"));
    m_atomStyleTypes.insert("nitrogen", new AtomStyle(1.55, "#3050F8"));
    m_atomStyleTypes.insert("oxygen", new AtomStyle(1.52, "#AA0000"));
    m_atomStyleTypes.insert("fluorine", new AtomStyle(1.35, "#90E050"));
    m_atomStyleTypes.insert("neon", new AtomStyle(1.54, "#3050F8"));
    m_atomStyleTypes.insert("sodium", new AtomStyle(2.27, "#AB5CF2"));
    m_atomStyleTypes.insert("magnesium", new AtomStyle(1.73, "#8AFF00"));
    m_atomStyleTypes.insert("aluminium", new AtomStyle(1.84, "#BFA6A6"));
    m_atomStyleTypes.insert("silicon", new AtomStyle(2.27, "#F0C8A0"));
    m_atomStyleTypes.insert("phosphorus", new AtomStyle(1.80, "#FF8000"));
    m_atomStyleTypes.insert("sulfur", new AtomStyle(1.80, "#FFFF30"));
    m_atomStyleTypes.insert("chlorine", new AtomStyle(1.75, "#1FF01F"));
    m_atomStyleTypes.insert("argon", new AtomStyle(1.88, "#80D1E3"));
    m_atomStyleTypes.insert("potassium", new AtomStyle(2.75, "#8F40D4"));
    m_atomStyleTypes.insert("calcium", new AtomStyle(2.31, "#3DFF00"));

    m_atomStyleTypes.insert("H", new AtomStyle(1.20, "#CCCCCC"));
    m_atomStyleTypes.insert("He", new AtomStyle(1.40, "#D9FFFF"));
    m_atomStyleTypes.insert("Li", new AtomStyle(1.82, "#CC80FF"));
    m_atomStyleTypes.insert("Be", new AtomStyle(1.53, "#C2FF00"));
    m_atomStyleTypes.insert("B", new AtomStyle(1.92, "#FFB5B5"));
    m_atomStyleTypes.insert("C", new AtomStyle(1.70, "#505050"));
    m_atomStyleTypes.insert("N", new AtomStyle(1.55, "#3050F8"));
    m_atomStyleTypes.insert("O", new AtomStyle(1.52, "#AA0000"));
    m_atomStyleTypes.insert("F", new AtomStyle(1.35, "#90E050"));
    m_atomStyleTypes.insert("Ne", new AtomStyle(1.54, "#3050F8"));
    m_atomStyleTypes.insert("Na", new AtomStyle(2.27, "#AB5CF2"));
    m_atomStyleTypes.insert("Mg", new AtomStyle(1.73, "#8AFF00"));
    m_atomStyleTypes.insert("Al", new AtomStyle(1.84, "#BFA6A6"));
    m_atomStyleTypes.insert("Si", new AtomStyle(2.27, "#F0C8A0"));
    m_atomStyleTypes.insert("P", new AtomStyle(1.80, "#FF8000"));
    m_atomStyleTypes.insert("S", new AtomStyle(1.80, "#FFFF30"));
    m_atomStyleTypes.insert("Cl", new AtomStyle(1.75, "#1FF01F"));
    m_atomStyleTypes.insert("Ar", new AtomStyle(1.88, "#80D1E3"));
    m_atomStyleTypes.insert("K", new AtomStyle(2.75, "#8F40D4"));
    m_atomStyleTypes.insert("Ca", new AtomStyle(2.31, "#3DFF00"));
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

    setDirty(true);
}

AtomData &Atoms::atomData()
{
    return m_atomData;
}

void Atoms::synchronizeRenderer() {
    if(!m_dirty) return;
    int numSpheres = m_sphereDataBytes.size() / sizeof(SphereVBOData);
    m_sphereData->setData(m_sphereDataBytes, numSpheres);
    setDirty(false);
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

bool Atoms::dirty() const
{
    return m_dirty;
}

void Atoms::setSort(bool sort)
{
    if (m_sort == sort)
        return;

    m_sort = sort;
    emit sortChanged(sort);
}

void Atoms::setDirty(bool dirty)
{
    if (m_dirty == dirty)
        return;

    m_dirty = dirty;
    emit dirtyChanged(dirty);
}
