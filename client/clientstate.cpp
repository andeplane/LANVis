#include "clientstate.h"
#include <cmath>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>
#include <QDebug>
#include <QDateTime>
#include "serversettings.h"

ClientState::ClientState(QObject *parent) : QObject(parent),
    m_dirty(false), m_chunksDirty(false), m_particlesDirty(false), m_maxNumberOfParticles(-1), m_sort(true),
    m_chunkSize(-1), m_lodDistance(250), m_lodLevels(0),
    m_serverSettings(new ServerSettings(this)), m_numThreads(4),
    m_timestep(0)
{

}

QVector3D ClientState::cameraPosition() const
{
    return m_cameraPosition;
}

void ClientState::save()
{
    QJsonObject json;
    QJsonArray array = { m_cameraPosition[0], m_cameraPosition[1], m_cameraPosition[2] };
    json["maxNumberOfParticles"] = QJsonValue::fromVariant(QVariant::fromValue<int>(m_maxNumberOfParticles));
    json["lodLevels"] = QJsonValue::fromVariant(QVariant::fromValue<int>(m_lodLevels));
    json["numThreads"] = QJsonValue::fromVariant(QVariant::fromValue<int>(m_numThreads));
    json["timestep"] = QJsonValue::fromVariant(QVariant::fromValue<int>(m_timestep));
    json["chunkSize"] = QJsonValue::fromVariant(QVariant::fromValue<float>(m_chunkSize));
    json["lodDistance"] = QJsonValue::fromVariant(QVariant::fromValue<float>(m_lodDistance));
    json["cameraPosition"] = array;
    json["sort"] = QJsonValue::fromVariant(QVariant::fromValue<bool>(m_sort));
    json["timestamp"] = QJsonValue::fromVariant(QVariant::fromValue<double>(QDateTime::currentDateTime().toMSecsSinceEpoch()));
    QJsonObject serverSettings;
    m_serverSettings->save(serverSettings);
    json["serverSettings"] = serverSettings;

    QFile file(m_fileName);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QJsonDocument saveObject(json);
        QTextStream stream(&file);
        stream << saveObject.toJson();
        file.close();
    }
}

bool ClientState::load()
{
    QFile file(m_fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray stateData = file.readAll();
    QJsonParseError error;
    QJsonDocument doc(QJsonDocument::fromJson(stateData, &error));
    if(doc.isNull()) return true;

    QJsonObject   obj = doc.object();
    m_serverSettings->load(obj["serverSettings"].toObject());

    QJsonArray    arr = obj["cameraPosition"].toArray();
    int maxNumberOfParticles = obj["maxNumberOfParticles"].toInt();
    float chunkSize = obj["chunkSize"].toDouble();
    float lodDistance = obj["lodDistance"].toDouble();
    int lodLevels = obj["lodLevels"].toInt();
    bool sort = obj["sort"].toBool();
    int timestep = obj["timestep"].toInt();
    m_numThreads = obj["numThreads"].toInt();

    QVector3D newCameraPositon;
    newCameraPositon[0] = arr[0].toDouble();
    newCameraPositon[1] = arr[1].toDouble();
    newCameraPositon[2] = arr[2].toDouble();
    float distanceToOldPositionSquared = (newCameraPositon - m_cameraPosition).lengthSquared();
    bool chunksDirty = fabs(m_chunkSize-chunkSize)>1.0;
    bool lodDirty = fabs(m_lodDistance-lodDistance)>1.0 || lodLevels != m_lodLevels;

    bool anyChanges = distanceToOldPositionSquared > 5 || maxNumberOfParticles!=m_maxNumberOfParticles || m_sort != sort || chunksDirty || lodDirty || m_timestep != timestep;
    if(!anyChanges) return true;

    m_timestep = timestep;
    m_maxNumberOfParticles = maxNumberOfParticles;
    m_cameraPosition = newCameraPositon;
    m_sort = sort;

    if(chunksDirty || lodDirty) {
        m_lodLevels = lodLevels;
        m_lodDistance = lodDistance;
        m_chunkSize = chunkSize;
        m_chunksDirty = true;
    }
    m_particlesDirty = true;
    return true;
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

    setDirty(true);
    m_cameraPosition = cameraPosition;
    emit cameraPositionChanged(cameraPosition);
}

void ClientState::setMaxNumberOfParticles(int maxNumberOfParticles)
{
    if (m_maxNumberOfParticles == maxNumberOfParticles)
        return;

    setDirty(true);
    m_maxNumberOfParticles = maxNumberOfParticles;
    emit maxNumberOfParticlesChanged(maxNumberOfParticles);
}

void ClientState::setSort(bool sort)
{
    if (m_sort == sort)
        return;

    setDirty(true);
    m_sort = sort;
    emit sortChanged(sort);
}

void ClientState::setChunkSize(float chunkSize)
{
    if (m_chunkSize == chunkSize)
        return;

    setDirty(true);
    m_chunkSize = chunkSize;
    emit chunkSizeChanged(chunkSize);
}

void ClientState::setLodDistance(float lodDistance)
{
    if (m_lodDistance == lodDistance)
        return;

    setDirty(true);
    m_lodDistance = lodDistance;
    emit lodDistanceChanged(lodDistance);
}

void ClientState::setLodLevels(int lodLevels)
{
    if (m_lodLevels == lodLevels)
        return;

    setDirty(true);
    m_lodLevels = lodLevels;
    emit lodLevelsChanged(lodLevels);
}

void ClientState::setFileName(QString fileName)
{
    if (m_fileName == fileName)
        return;

    setDirty(true);
    m_fileName = fileName;
    emit fileNameChanged(fileName);
    load();
}

void ClientState::setServerSettings(ServerSettings *serverSettings)
{
    if (m_serverSettings == serverSettings)
        return;

    setDirty(true);
    m_serverSettings = serverSettings;
    emit serverSettingsChanged(serverSettings);
}

void ClientState::setNumThreads(int numThreads)
{
    if (m_numThreads == numThreads)
        return;

    m_numThreads = numThreads;
    emit numThreadsChanged(numThreads);
}

void ClientState::setTimestep(int timestep)
{
    if (m_timestep == timestep)
        return;

    setDirty(true);
    m_timestep = timestep;
    emit timestepChanged(timestep);
}

bool ClientState::dirty() const
{
    return m_dirty;
}

void ClientState::setDirty(bool dirty)
{
    m_dirty = dirty;
}

ServerSettings *ClientState::serverSettings() const
{
    return m_serverSettings;
}

int ClientState::numThreads() const
{
    return m_numThreads;
}

int ClientState::timestep() const
{
    return m_timestep;
}

void ClientState::setParticlesDirty(bool particlesDirty)
{
    m_particlesDirty = particlesDirty;
}

QString ClientState::fileName() const
{
    return m_fileName;
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
