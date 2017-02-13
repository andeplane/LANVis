#ifndef IMPORTER_H
#define IMPORTER_H

#include <QFile>
#include <QObject>

class ParticleImporter
{
public:
    ParticleImporter();
    virtual bool checkFileFormat(QFile &file) = 0;
    virtual bool readFile(QString fileName) = 0;

signals:

public slots:
};

#endif // IMPORTER_H
