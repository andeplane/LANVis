#include "state.h"

#include <QFile>
#include <QTextStream>

State::State(QObject *parent) : QObject(parent)
{

}

void State::sortChunks()
{
    std::sort(m_chunkPtrs.begin(), m_chunkPtrs.end(),
              [&](const Chunk* a, const Chunk* b)
    {
        float da = a->minDistanceTo(m_cameraPosition);
        float db = b->minDistanceTo(m_cameraPosition);
        return da < db;
    });
}

void State::save(QString fileName)
{
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QTextStream stream(&file);
        QJsonObject json;

        json["timestamp"] = QJsonValue::fromVariant(QVariant::fromValue<double>(QDateTime::currentDateTime().toMSecsSinceEpoch()));
        json["particleCount"] = QJsonValue::fromVariant(QVariant::fromValue<int>(m_particles.size()));
        json["binaryFileName"] = m_dataFileName;
        json["lockFileName"] = m_lockFileName;
        json["boundingBoxMin"] = QJsonArray({m_boundingBoxMin[0], m_boundingBoxMin[1], m_boundingBoxMin[2]});
        json["boundingBoxMax"] = QJsonArray({m_boundingBoxMax[0], m_boundingBoxMax[1], m_boundingBoxMax[2]});

        QJsonDocument saveObject(json);
        stream << saveObject.toJson();
        file.close();
    } else {
        qDebug() << "Could not open file " << fileName;
    }
}
