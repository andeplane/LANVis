#ifndef XYZIMPORTER_H
#define XYZIMPORTER_H

#include "particleimporter.h"

class XYZImporter : public ParticleImporter
{
public:
    XYZImporter();

    // ParticleImporter interface
public:
    bool checkFileFormat(QFile &file) override;
    bool readFile(QString fileName) override;
};

#endif // XYZIMPORTER_H
