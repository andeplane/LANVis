#include "clientstate.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>
#include <QDebug>

ClientState::ClientState(QObject *parent) : QObject(parent), m_maxNumberOfAtoms(300000), m_sort(true), m_chunkSize(50)
{

}

QVector3D ClientState::cameraPosition() const
{
    return m_cameraPosition;
}

void ClientState::save(QString fileName)
{
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QTextStream stream(&file);
        QJsonObject json;
        QJsonArray array = { m_cameraPosition[0], m_cameraPosition[1], m_cameraPosition[2] };
        json["maxNumberOfAtoms"] = QJsonValue::fromVariant(QVariant::fromValue<int>(m_maxNumberOfAtoms));
        json["chunkSize"] = QJsonValue::fromVariant(QVariant::fromValue<float>(m_chunkSize));
        json["cameraPosition"] = array;
        json["sort"] = QJsonValue::fromVariant(QVariant::fromValue<bool>(m_sort));
        QJsonDocument saveObject(json);
        stream << saveObject.toJson();
    }
    file.close();
}

int ClientState::maxNumberOfAtoms() const
{
    return m_maxNumberOfAtoms;
}

bool ClientState::sort() const
{
    return m_sort;
}

float ClientState::chunkSize() const
{
    return m_chunkSize;
}

void ClientState::setCameraPosition(QVector3D cameraPosition)
{
    if (m_cameraPosition == cameraPosition)
        return;

    m_cameraPosition = cameraPosition;
    emit cameraPositionChanged(cameraPosition);
}

void ClientState::setMaxNumberOfAtoms(int maxNumberOfAtoms)
{
    if (m_maxNumberOfAtoms == maxNumberOfAtoms)
        return;

    m_maxNumberOfAtoms = maxNumberOfAtoms;
    emit maxNumberOfAtomsChanged(maxNumberOfAtoms);
}

void ClientState::setSort(bool sort)
{
    if (m_sort == sort)
        return;

    m_sort = sort;
    emit sortChanged(sort);
}

void ClientState::setChunkSize(float chunkSize)
{
    if (m_chunkSize == chunkSize)
        return;

    m_chunkSize = chunkSize;
    emit chunkSizeChanged(chunkSize);
}
