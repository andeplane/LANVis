#include "serversettings.h"
#include <QDebug>
#include <QJsonArray>

ServerSettings::ServerSettings(QObject *parent) : QObject(parent)
{

}

QString ServerSettings::inputFile() const
{
    return m_inputFile;
}

QString ServerSettings::inputFileType() const
{
    return m_inputFileType;
}

void ServerSettings::load(const QJsonObject &json)
{
    m_inputFile = json["inputFile"].toString();
    m_inputFileType = json["inputFileType"].toString();
    QStringList propertyNames;
    for(auto obj : json["propertyNames"].toArray()) {
        QString propertyName = obj.toString();
        propertyNames.push_back(propertyName);
    }
    setPropertyNames(propertyNames);
}

void ServerSettings::save(QJsonObject &json) const
{
    json["inputFile"] = m_inputFile;
    json["inputFileType"] = m_inputFileType;
    QJsonArray propertyNames;
    for(QString propertyName : m_propertyNames) {
        propertyNames.append(propertyName);
    }
    json["propertyNames"] = propertyNames;
}

QStringList ServerSettings::propertyNames() const
{
    return m_propertyNames;
}

bool ServerSettings::operator ==(const ServerSettings &settings) const
{
    return settings.inputFile()==m_inputFile &&
           settings.inputFileType()==m_inputFileType &&
            settings.propertyNames()==m_propertyNames;
}

bool ServerSettings::operator !=(const ServerSettings &settings) const
{
    return settings.inputFile()!=m_inputFile ||
           settings.inputFileType()!=m_inputFileType ||
            settings.propertyNames()!=m_propertyNames;
}

void ServerSettings::setInputFile(QString inputFile)
{
    if (m_inputFile == inputFile)
        return;

    m_inputFile = inputFile;
    emit inputFileChanged(inputFile);
}

void ServerSettings::setInputFileType(QString inputFileType)
{
    if (m_inputFileType == inputFileType)
        return;

    m_inputFileType = inputFileType;
    emit inputFileTypeChanged(inputFileType);
}

void ServerSettings::setPropertyNames(QStringList propertyNames)
{
    if (m_propertyNames == propertyNames)
        return;

    m_propertyNames = propertyNames;
    emit propertyNamesChanged(propertyNames);
}
