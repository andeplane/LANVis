#include "clientstate.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>
#include <QDebug>

ClientState::ClientState(QObject *parent) : QObject(parent), m_maxNumberOfAtoms(300000)
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
        json["cameraPosition"] = array;
        QJsonDocument saveObject(json);
        stream << saveObject.toJson();
    }
    file.close();
}

int ClientState::maxNumberOfAtoms() const
{
    return m_maxNumberOfAtoms;
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
