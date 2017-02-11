#include "clientstate.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>
#include <QDebug>

ClientState::ClientState(QObject *parent) : QObject(parent),
    m_maxNumberOfParticles(300000), m_sort(true), m_chunkSize(50), m_lodDistance(250), m_lodLevels(5)
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
        json["maxNumberOfParticles"] = QJsonValue::fromVariant(QVariant::fromValue<int>(m_maxNumberOfParticles));
        json["lodLevels"] = QJsonValue::fromVariant(QVariant::fromValue<int>(m_lodLevels));
        json["chunkSize"] = QJsonValue::fromVariant(QVariant::fromValue<float>(m_chunkSize));
        json["lodDistance"] = QJsonValue::fromVariant(QVariant::fromValue<float>(m_lodDistance));
        json["cameraPosition"] = array;
        json["sort"] = QJsonValue::fromVariant(QVariant::fromValue<bool>(m_sort));
        QJsonDocument saveObject(json);
        stream << saveObject.toJson();
    }
    file.close();
}

void ClientState::load(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open state file");
        return false;
    }

    QByteArray stateData = file.readAll();
    QJsonParseError error;
    QJsonDocument doc(QJsonDocument::fromJson(stateData, &error));
    if(doc.isNull()) return false;

    QJsonObject   obj = doc.object();
    QJsonArray    arr = obj["cameraPosition"].toArray();
    int maxNumberOfParticles = obj["maxNumberOfParticles"].toInt();
    float chunkSize = obj["chunkSize"].toDouble();
    float lodDistance = obj["lodDistance"].toDouble();
    int lodLevels = obj["lodLevels"].toInt();
    bool sort = obj["sort"].toBool();

    QVector3D newCameraPositon;
    newCameraPositon[0] = arr[0].toDouble();
    newCameraPositon[1] = arr[1].toDouble();
    newCameraPositon[2] = arr[2].toDouble();
    float distanceToOldPositionSquared = (newCameraPositon - m_cameraPosition).lengthSquared();
    bool chunksDirty = fabs(m_chunkSize-chunkSize)>1.0;
    bool lodDirty = fabs(m_lodDistance-lodDistance)>1.0 || lodLevels != m_lodLevels;

    bool anyChanges = distanceToOldPositionSquared > 5 || maxNumberOfParticles!=m_maxNumberOfParticles || m_sort != sort || chunksDirty || lodDirty;
    if(!anyChanges) return false;

    m_maxNumberOfParticles = maxNumberOfParticles;
    m_cameraPosition = newCameraPositon;
    m_sort = sort;

    if(chunksDirty || lodDirty) {
        m_lodLevels = lodLevels;
        m_lodDistance = lodDistance;
        m_chunkSize = chunkSize;
        m_chunksDirty = true;
    }
}

int ClientState::maxNumberOfParticles() const
{
    return m_maxNumberOfParticles;
}

bool ClientState::sort() const
{
    return m_sort;
}

float ClientState::chunkSize() const
{
    return m_chunkSize;
}

float ClientState::lodDistance() const
{
    return m_lodDistance;
}

int ClientState::lodLevels() const
{
    return m_lodLevels;
}

void ClientState::setCameraPosition(QVector3D cameraPosition)
{
    if (m_cameraPosition == cameraPosition)
        return;

    m_cameraPosition = cameraPosition;
    emit cameraPositionChanged(cameraPosition);
}

void ClientState::setMaxNumberOfParticles(int maxNumberOfParticles)
{
    if (m_maxNumberOfParticles == maxNumberOfParticles)
        return;

    m_maxNumberOfParticles = maxNumberOfParticles;
    emit maxNumberOfParticlesChanged(maxNumberOfParticles);
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

void ClientState::setLodDistance(float lodDistance)
{
    if (m_lodDistance == lodDistance)
        return;

    m_lodDistance = lodDistance;
    emit lodDistanceChanged(lodDistance);
}

void ClientState::setLodLevels(int lodLevels)
{
    if (m_lodLevels == lodLevels)
        return;

    m_lodLevels = lodLevels;
    emit lodLevelsChanged(lodLevels);
}

void ClientState::setParticlesDirty(bool particlesDirty)
{
    m_particlesDirty = particlesDirty;
}

void ClientState::setChunksDirty(bool chunksDirty)
{
    m_chunksDirty = chunksDirty;
}

bool ClientState::particlesDirty() const
{
    return m_particlesDirty;
}

bool ClientState::chunksDirty() const
{
    return m_chunksDirty;
}
