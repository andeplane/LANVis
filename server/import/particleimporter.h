#ifndef IMPORTER_H
#define IMPORTER_H

#include <QFile>
#include <QObject>

class ParticleImporter : public QObject
{
    Q_OBJECT
public:
    explicit Importer(QObject *parent = 0);
    virtual bool checkFileFormat(QFile &file) = 0;
    virtual bool readFile(QString fileName) = 0;

signals:

public slots:
};

#endif // IMPORTER_H
