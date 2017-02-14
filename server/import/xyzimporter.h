#ifndef XYZIMPORTER_H
#define XYZIMPORTER_H

#include "particleimporter.h"

class XYZImporter : public ParticleImporter<XYZImporter>
{

public:
    XYZImporter();

    // ParticleImporter interface
public:
    static bool doCheckFileFormat(QFile &file);
    static bool doReadFile(QString fileName, QVector<State *> &states, ClientState &clientState);
};

#endif // XYZIMPORTER_H
