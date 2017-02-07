#include "atoms.h"

Atoms::Atoms(QObject *parent) : QObject(parent),
    m_sphereData(nullptr), m_bondData(nullptr), m_sort(false), m_dirty(false), m_sphereScale(0.5)
{
    setDefaultStyle();
}

void Atoms::setDefaultStyle() {
    m_atomStyles.insert("hydrogen", new AtomStyle(1.20, "#CCCCCC"));
    m_atomStyles.insert("helium", new AtomStyle(1.40, "#D9FFFF"));
    m_atomStyles.insert("lithium", new AtomStyle(1.82, "#CC80FF"));
    m_atomStyles.insert("beryllium", new AtomStyle(1.53, "#C2FF00"));
    m_atomStyles.insert("boron", new AtomStyle(1.92, "#FFB5B5"));
    m_atomStyles.insert("carbon", new AtomStyle(1.70, "#505050"));
    m_atomStyles.insert("nitrogen", new AtomStyle(1.55, "#3050F8"));
    m_atomStyles.insert("oxygen", new AtomStyle(1.52, "#AA0000"));
    m_atomStyles.insert("fluorine", new AtomStyle(1.35, "#90E050"));
    m_atomStyles.insert("neon", new AtomStyle(1.54, "#3050F8"));
    m_atomStyles.insert("sodium", new AtomStyle(2.27, "#AB5CF2"));
    m_atomStyles.insert("magnesium", new AtomStyle(1.73, "#8AFF00"));
    m_atomStyles.insert("aluminium", new AtomStyle(1.84, "#BFA6A6"));
    m_atomStyles.insert("silicon", new AtomStyle(2.27, "#F0C8A0"));
    m_atomStyles.insert("phosphorus", new AtomStyle(1.80, "#FF8000"));
    m_atomStyles.insert("sulfur", new AtomStyle(1.80, "#FFFF30"));
    m_atomStyles.insert("chlorine", new AtomStyle(1.75, "#1FF01F"));
    m_atomStyles.insert("argon", new AtomStyle(1.88, "#80D1E3"));
    m_atomStyles.insert("potassium", new AtomStyle(2.75, "#8F40D4"));
    m_atomStyles.insert("calcium", new AtomStyle(2.31, "#3DFF00"));

    m_atomStyles.insert("H", new AtomStyle(1.20, "#CCCCCC"));
    m_atomStyles.insert("He", new AtomStyle(1.40, "#D9FFFF"));
    m_atomStyles.insert("Li", new AtomStyle(1.82, "#CC80FF"));
    m_atomStyles.insert("Be", new AtomStyle(1.53, "#C2FF00"));
    m_atomStyles.insert("B", new AtomStyle(1.92, "#FFB5B5"));
    m_atomStyles.insert("C", new AtomStyle(1.70, "#505050"));
    m_atomStyles.insert("N", new AtomStyle(1.55, "#3050F8"));
    m_atomStyles.insert("O", new AtomStyle(1.52, "#AA0000"));
    m_atomStyles.insert("F", new AtomStyle(1.35, "#90E050"));
    m_atomStyles.insert("Ne", new AtomStyle(1.54, "#3050F8"));
    m_atomStyles.insert("Na", new AtomStyle(2.27, "#AB5CF2"));
    m_atomStyles.insert("Mg", new AtomStyle(1.73, "#8AFF00"));
    m_atomStyles.insert("Al", new AtomStyle(1.84, "#BFA6A6"));
    m_atomStyles.insert("Si", new AtomStyle(2.27, "#F0C8A0"));
    m_atomStyles.insert("P", new AtomStyle(1.80, "#FF8000"));
    m_atomStyles.insert("S", new AtomStyle(1.80, "#FFFF30"));
    m_atomStyles.insert("Cl", new AtomStyle(1.75, "#1FF01F"));
    m_atomStyles.insert("Ar", new AtomStyle(1.88, "#80D1E3"));
    m_atomStyles.insert("K", new AtomStyle(2.75, "#8F40D4"));
    m_atomStyles.insert("Ca", new AtomStyle(2.31, "#3DFF00"));
}

void Atoms::generateSphereData() {

    m_sphereDataBytes.resize(m_atomData.size() * sizeof(SphereVBOData));
    SphereVBOData *vboData = reinterpret_cast<SphereVBOData *>(m_sphereDataBytes.data());
    for(int i=0; i<m_atomData.size(); i++) {
        SphereVBOData &vbo = vboData[i];
        vbo.position = m_atomData.positions[i];
        vbo.color = m_atomData.colors[i];
        vbo.radius = m_atomData.radii[i] * m_sphereScale;
    }
    setDirty(true);
}

AtomData &Atoms::atomData()
{
    return m_atomData;
}

void Atoms::setData(const QVector<QVector3D> &positions, const QVector<QString> &types)
{
    if(positions.size() != types.size()) {
        qFatal("Error in Atoms::setData: positions.size() != types.size().");
    }

    m_atomData.resize(positions.size());
    m_atomData.positions = positions;
    for(int i=0; i<m_atomData.size(); i++) {
        float radius = 1.0;
        QVector3D color(1.0, 0.9, 0.8);

        if(m_atomStyles.contains(types[i])) {
            radius = m_atomStyles[types[i]]->radius;
            color[0] = m_atomStyles[types[i]]->color.redF();
            color[1] = m_atomStyles[types[i]]->color.greenF();
            color[2] = m_atomStyles[types[i]]->color.blueF();
        }
        m_atomData.radii[i] = radius;
        m_atomData.colors[i] = color;
    }
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

void Atoms::setSphereScale(float sphereScale)
{
    if (m_sphereScale == sphereScale)
            return;

        m_sphereScale = sphereScale;
        emit sphereScaleChanged(sphereScale);
}

void Atoms::setSphereData(SphereData *sphereData)
{
    m_sphereData = sphereData;
}

float Atoms::sphereScale() const
{
    return m_sphereScale;
}
