#include "particles.h"

#include <QFile>

Particles::Particles(QObject *parent) : QObject(parent),
    m_sphereData(nullptr), m_dirty(false), m_sphereScale(0.5)
{

}

void Particles::generateSphereData() {
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

bool Particles::loadBinary(QString fileName)
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

void Particles::synchronizeRenderer() {
    if(!m_dirty) return;
    int numSpheres = m_sphereDataBytes.size() / sizeof(SphereVBOData);
    m_sphereData->setData(m_sphereDataBytes, numSpheres);
    emit countChanged(m_particles.size());
    setDirty(false);
}

SphereData *Particles::sphereData() const
{
    return m_sphereData;
}

bool Particles::dirty() const
{
    return m_dirty;
}

void Particles::setDirty(bool dirty)
{
    if (m_dirty == dirty)
        return;

    m_dirty = dirty;
    emit dirtyChanged(dirty);
}

void Particles::setSphereScale(float sphereScale)
{
    if (m_sphereScale == sphereScale)
            return;

        m_sphereScale = sphereScale;
        emit sphereScaleChanged(sphereScale);
}

void Particles::setBoundingBoxMin(QVector3D boundingBoxMin)
{
    if (m_boundingBoxMin == boundingBoxMin)
            return;

        m_boundingBoxMin = boundingBoxMin;
        emit boundingBoxMinChanged(boundingBoxMin);
}

void Particles::setBoundingBoxMax(QVector3D boundingBoxMax)
{
    if (m_boundingBoxMax == boundingBoxMax)
            return;

        m_boundingBoxMax = boundingBoxMax;
        emit boundingBoxMaxChanged(boundingBoxMax);
}

void Particles::setSphereData(SphereData *sphereData)
{
    m_sphereData = sphereData;
}

float Particles::sphereScale() const
{
    return m_sphereScale;
}

int Particles::count() const
{
    return m_particles.size();
}

QVector3D Particles::boundingBoxMin() const
{
    return m_boundingBoxMin;
}

QVector3D Particles::boundingBoxMax() const
{
    return m_boundingBoxMax;
}

Particle &Particles::operator()(int index)
{
    return m_particles[index];
}

QVector<Particle> &Particles::get()
{
    return m_particles;
}
