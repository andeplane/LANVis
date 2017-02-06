#include "state.h"
#include "xyzreader.h"
#include <QString>

State::State(QObject *parent) : QObject(parent),
    m_atoms(new Atoms())
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
        QString xyzFilename = object["xyzFilename"].toString();
        XYZReader reader;
        reader.readFile(xyzFilename);
        AtomData &atomData = m_atoms->atomData();
        const QVector<QString> &types = reader.types();
        const QVector<QVector3D> &points = reader.points();
        atomData.resize(points.size());
        atomData.positions = points;

        m_atoms->setDirty(true);
        m_timestamp = timestamp;
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
