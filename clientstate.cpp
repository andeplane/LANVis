#include "clientstate.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>
#include <QDebug>

ClientState::ClientState(QObject *parent) : QObject(parent)
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
        json["cameraPosition"] = array;
        QJsonDocument saveObject(json);
        stream << saveObject.toJson();
    }
    file.close();
}

void ClientState::setCameraPosition(QVector3D cameraPosition)
{
    if (m_cameraPosition == cameraPosition)
        return;

    m_cameraPosition = cameraPosition;
    emit cameraPositionChanged(cameraPosition);
}
