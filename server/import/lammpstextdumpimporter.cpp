#include "lammpstextdumpimporter.h"

#include <QTextStream>
#include <QDebug>

LAMMPSTextDumpImporter::LAMMPSTextDumpImporter()
{

}

bool LAMMPSTextDumpImporter::checkFileFormat(QFile &file)
{
    QTextStream stream(&file);
    // Read first line.
    QString line = stream.readLine(15);

    if(line.startsWith("ITEM: TIMESTEP")) {
        return true;
    }


    return false;
}


bool LAMMPSTextDumpImporter::readFile(QString fileName)
{
//    QFile file(fileName);
//    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        qDebug() << "Could not open file " << fileName;
//        return false;
//    }

//    if(!checkFileFormat(fileName)) return false;
//    file.seek(0);

//    QTextStream stream(file);
//    while(!stream.atEnd()) {
//        QString line = stream.readLine();

//    }

}
