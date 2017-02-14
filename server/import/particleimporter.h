#ifndef IMPORTER_H
#define IMPORTER_H

#include <QFile>
#include <QObject>
#include "../client/clientstate.h"

template< class DerivedType >
class ParticleImporter
{
public:
    static bool checkFileFormat(QFile &file) { return DerivedType::doCheckFileFormat(file); }
    static bool readFile(QString fileName, QVector<class State*> &states, ClientState &clientState) { return DerivedType::doReadFile(fileName, states, clientState); }

signals:

public slots:
};

#endif // IMPORTER_H
