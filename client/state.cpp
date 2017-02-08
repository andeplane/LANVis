#include "state.h"
#include <QJsonArray>
#include <QLockFile>
#include <QString>

State::State(QObject *parent) : QObject(parent),
    m_atoms(new Atoms(this))
{

}

int State::timestamp() const
{
    return m_timestamp;
}

Atoms *State::atoms() const
{
    return m_atoms;
}

void State::update(const QJsonObject &object)
{
    double timestamp = object["timestamp"].toDouble();
    if(timestamp > m_timestamp) {
        bool ok = false;
        QLockFile lockFile(object["lockFileName"].toString());

        if(lockFile.tryLock(200)) {
            QJsonArray boundingBoxMinA = object["boundingBoxMin"].toArray();
            QJsonArray boundingBoxMaxA = object["boundingBoxMax"].toArray();
            QVector3D boundingBoxMin(boundingBoxMinA[0].toDouble(), boundingBoxMinA[1].toDouble(), boundingBoxMinA[2].toDouble());
            QVector3D boundingBoxMax(boundingBoxMaxA[0].toDouble(), boundingBoxMaxA[1].toDouble(), boundingBoxMaxA[2].toDouble());
            m_atoms->setBoundingBoxMin(boundingBoxMin);
            m_atoms->setBoundingBoxMax(boundingBoxMax);
            m_timestamp = timestamp;
            if(object.contains("binaryFileName")) {
                ok = m_atoms->loadBinary(object["binaryFileName"].toString());
            }

            if(ok) m_atoms->generateSphereData();
            lockFile.unlock();
        }
    }
}

void State::setTimestamp(int timestamp)
{
    if (m_timestamp == timestamp)
        return;

    m_timestamp = timestamp;
    emit timestampChanged(timestamp);
}

void State::setAtoms(Atoms *atoms)
{
    if (m_atoms == atoms)
        return;

    m_atoms = atoms;
    emit atomsChanged(atoms);
}
