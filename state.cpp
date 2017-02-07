#include "state.h"
#include "xyzreader.h"
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
    int timestamp = object["timestamp"].toInt();
    if(timestamp > m_timestamp) {
        m_timestamp = timestamp;
        QString xyzFilename = object["xyzFileName"].toString();
        XYZReader reader;
        reader.readFile(xyzFilename);
        m_atoms->setData(reader.points(), reader.types());
        m_atoms->generateSphereData();
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
