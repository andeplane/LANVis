#include "atoms.h"
#include "particle.h"

#include <QFile>

Atoms::Atoms(QObject *parent) : QObject(parent),
    m_sphereData(nullptr), m_dirty(false), m_sphereScale(0.5)
{

}

void Atoms::generateSphereData() {
    m_sphereDataBytes.resize(m_particles.size() * sizeof(SphereVBOData));
    SphereVBOData *vboData = reinterpret_cast<SphereVBOData *>(m_sphereDataBytes.data());
    for(int i=0; i<m_particles.size(); i++) {
        SphereVBOData &vbo = vboData[i];
        vbo.position = m_particles[i].position;
        vbo.color = m_particles[i].color;
        vbo.radius = m_particles[i].radius * m_sphereScale;
    }
    setDirty(true);
}

bool Atoms::loadBinary(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not open file " << fileName;
        return false;
    }

    QByteArray ba = file.readAll();
    file.close();
    Particle *particles = reinterpret_cast<Particle *>(ba.data());
    int numParticles = ba.length() / sizeof(Particle);
    m_particles.resize(numParticles);
    memcpy(&m_particles.front(), particles, numParticles*sizeof(Particle));
    return true;
}

void Atoms::synchronizeRenderer() {
    if(!m_dirty) return;
    int numSpheres = m_sphereDataBytes.size() / sizeof(SphereVBOData);
    m_sphereData->setData(m_sphereDataBytes, numSpheres);
    emit countChanged(m_particles.size());
    setDirty(false);
}

SphereData *Atoms::sphereData() const
{
    return m_sphereData;
}

bool Atoms::dirty() const
{
    return m_dirty;
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

void Atoms::setBoundingBoxMin(QVector3D boundingBoxMin)
{
    if (m_boundingBoxMin == boundingBoxMin)
            return;

        m_boundingBoxMin = boundingBoxMin;
        emit boundingBoxMinChanged(boundingBoxMin);
}

void Atoms::setBoundingBoxMax(QVector3D boundingBoxMax)
{
    if (m_boundingBoxMax == boundingBoxMax)
            return;

        m_boundingBoxMax = boundingBoxMax;
        emit boundingBoxMaxChanged(boundingBoxMax);
}

void Atoms::setSphereData(SphereData *sphereData)
{
    m_sphereData = sphereData;
}

float Atoms::sphereScale() const
{
    return m_sphereScale;
}

int Atoms::count() const
{
    return m_particles.size();
}

QVector3D Atoms::boundingBoxMin() const
{
    return m_boundingBoxMin;
}

QVector3D Atoms::boundingBoxMax() const
{
    return m_boundingBoxMax;
}
