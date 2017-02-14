#ifndef IMPORTER_H
#define IMPORTER_H

#include <QFile>
#include <QObject>

template< class DerivedType >
class ParticleImporter
{
public:
    static bool checkFileFormat(QFile &file) { return DerivedType::doCheckFileFormat(file); }
    static bool readFile(QString fileName, QVector<class State*> &states) { return DerivedType::doReadFile(fileName, states); }

signals:

public slots:
};

#endif // IMPORTER_H
