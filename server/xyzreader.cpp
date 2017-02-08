#include "xyzreader.h"
#include <QFile>
#include <QDebug>
#include <QUrl>
XYZReader::XYZReader(QObject *parent) : QObject(parent)
{

}

const QVector<QString> &XYZReader::types() const
{
    return m_types;
}

const QVector<QVector3D> &XYZReader::positions() const
{
    return m_positions;
}

bool XYZReader::readFile(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        file.setFileName(QUrl(filename).toLocalFile());
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Could not open file "+filename;
            return false;
        }
    }

    int numberOfAtoms = 0;
    int lineNumber = 0;
    int positionCount = 0;
    // Set lengths to zero and update based on the maximum measured coordinate in the atoms. Add a small epsilon after we're done
    // so x / m_lx always is smaller than the number of voxels in that dimension.
    m_types.resize(0);
    m_positions.resize(0);
    while (!file.atEnd()) {
        QString line = file.readLine();
        if(++lineNumber == 2) continue; // second line which is a comment, ignore.

        QStringList splitted = line.split(QRegExp("\\s+"));
        if(splitted.count() == 2) {
            bool ok;
            numberOfAtoms = splitted[0].toDouble(&ok);

            if(!ok) {
                qDebug() << QString("Error, tried to read number of atoms, but line '%1' didn't cast well.").arg(line);
                return false;
            }
            m_positions.resize(numberOfAtoms);
            m_types.resize(numberOfAtoms);
        } else if(splitted.count() == 5) {
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
            if(positionCount<m_positions.size()) {
                m_types [positionCount]    = atomType;
                m_positions[positionCount][0] = x;
                m_positions[positionCount][1] = y;
                m_positions[positionCount][2] = z;
                positionCount++;
            } else break; // If this is a multi timestep xyz-file, just ignore the rest
        }
    }

    if(positionCount != m_positions.size()) {
        qDebug() << QString("Error, could not parse XYZ file");
        return false;
    }
    return true;
}
