#ifndef SERVERSETTINGS_H
#define SERVERSETTINGS_H

#include <QJsonObject>
#include <QObject>

class ServerSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString inputFile READ inputFile WRITE setInputFile NOTIFY inputFileChanged)
    Q_PROPERTY(QString inputFileType READ inputFileType WRITE setInputFileType NOTIFY inputFileTypeChanged)
public:
    explicit ServerSettings(QObject *parent = nullptr);
    QString inputFile() const;
    QString inputFileType() const;
    void load(const QJsonObject &json);
    void save(QJsonObject &json) const;

public slots:
    void setInputFile(QString inputFile);
    void setInputFileType(QString inputFileType);

signals:
    void inputFileChanged(QString inputFile);
    void inputFileTypeChanged(QString inputFileType);

private:
    QString m_inputFile; // i.e. /projects/file.xyz
    QString m_inputFileType; // "xyz", "lammpsBinaryDump", "lammpsTextDump", "xyzBinary"
};

#endif // SERVERSETTINGS_H
