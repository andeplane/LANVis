#include "serversettings.h"

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
}

void ServerSettings::save(QJsonObject &json) const
{
    json["inputFile"] = m_inputFile;
    json["inputFileType"] = m_inputFileType;
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
