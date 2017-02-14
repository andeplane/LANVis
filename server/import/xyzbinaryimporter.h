#ifndef XYZBINARYIMPORTER_H
#define XYZBINARYIMPORTER_H

#include "particleimporter.h"

class XYZBinaryImporter : public ParticleImporter<XYZBinaryImporter>
{
public:
    XYZBinaryImporter();
    static bool doCheckFileFormat(QFile &file);
    static bool doReadFile(QString fileName, QVector<class State *> &states, ClientState &clientState);
};

#endif // XYZBINARYIMPORTER_H
