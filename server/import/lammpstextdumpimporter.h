#ifndef LAMMPSTEXTDUMPIMPORTER_H
#define LAMMPSTEXTDUMPIMPORTER_H

#include "particleimporter.h"

class LAMMPSTextDumpImporter : public ParticleImporter
{
public:
    LAMMPSTextDumpImporter();

    // ParticleImporter interface
public:
    bool checkFileFormat(QFile &file) override;
    bool readFile(QString fileName) override;
};

#endif // LAMMPSTEXTDUMPIMPORTER_H
