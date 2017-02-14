#include "xyzimporter.h"
#include "state.h"
#include <QDebug>
#include <QVector3D>

XYZImporter::XYZImporter()
{

}


bool XYZImporter::doCheckFileFormat(QFile &file)
{

}

bool XYZImporter::doReadFile(QString fileName, QVector<State*> &states)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open file "+fileName;
        return false;
    }
    qDebug() << "Opened XYZ file " << fileName;
    QVector3D min, max, origo, size;

    int numberOfParticles = 0;
    int lineNumberInTimestep = 0;
    int positionCount = 0;
    // Set lengths to zero and update based on the maximum measured coordinate in the atoms. Add a small epsilon after we're done
    // so x / m_lx always is smaller than the number of voxels in that dimension.
    std::vector<QVector3D> positions;
    std::vector<QString> types;

    bool nextTimestep = true;

    while (!file.atEnd()) {
        QString line = file.readLine();
        if(++lineNumberInTimestep == 2) continue; // second line which is a comment, ignore.

        QStringList splitted = line.split(QRegExp("\\s+"), QString::SplitBehavior::SkipEmptyParts);
        if(lineNumberInTimestep == 1) {
            // Number of atoms line, new timestep
            max = QVector3D(-1e9,-1e9,-1e9);
            min = QVector3D(1e9,1e9,1e9);

            bool ok;
            numberOfParticles = splitted[0].toInt(&ok);
            if(!ok) {
                qDebug() << QString("Error, tried to read number of atoms, but line '%1' didn't cast well.").arg(line);
                return false;
            }
            qDebug() << "Found " << numberOfParticles << " atoms in file. Now reading positions.";
            positions.resize(numberOfParticles);
            types.resize(numberOfParticles);
        } else if(splitted.count() == 4) {
            // AtomType x y z \n
            QString atomType = splitted[0];

            bool x_ok, y_ok, z_ok;
            float x = splitted[1].toFloat(&x_ok);
            float y = splitted[2].toFloat(&y_ok);
            float z = splitted[3].toFloat(&z_ok);
            if(!x_ok || !y_ok || !z_ok) {
                qDebug() << QString("Error, tried to read atom line, but '%1' didn't cast well.").arg(line);
                return false;
            }

            if(positionCount<numberOfParticles) {
                min[0] = std::min(min[0], x);
                min[1] = std::min(min[1], y);
                min[2] = std::min(min[2], z);

                max[0] = std::max(max[0], x);
                max[1] = std::max(max[1], y);
                max[2] = std::max(max[2], z);

                types[positionCount]    = atomType;
                positions[positionCount][0] = x;
                positions[positionCount][1] = y;
                positions[positionCount][2] = z;
                positionCount++;
            }

            if(positionCount==numberOfParticles) {
                positionCount = 0;
                origo = min;
                size = max-min;
                nextTimestep = true;
                lineNumberInTimestep = 0;
                State *state = new State();
                state->addParticles(positions, types, origo, size);
                states.push_back(state);

                // Next timestep. Skip for now
                break;
            }
        }
    }

    return true;
}
