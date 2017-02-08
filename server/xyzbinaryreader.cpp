#include "xyzbinaryreader.h"
#include <QDebug>
#include <QFile>

XYZBinaryReader::XYZBinaryReader(QObject *parent) : QObject(parent), m_numParticles(0)
{

}

bool XYZBinaryReader::readFile(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Error, could not open file " << fileName;
        return false;
    }

    QByteArray bytes = file.readAll();
    float *values = reinterpret_cast<float*>(bytes.data());
    int numColumns = values[0];
    qDebug() << "Read " << bytes.length() << " bytes.";
    qDebug() << "which is " << numColumns << " columns.";
    m_numParticles = bytes.length() / sizeof(float) / numColumns;
    qDebug() << "and " << m_numParticles << " particles.";
    m_columns.resize(numColumns);
    for(int column=0; column<numColumns; column++) {
        m_columns[column].resize(m_numParticles);
    }

    int numFloats = numColumns*m_numParticles+1;
    for(int particleIndex=0; particleIndex<m_numParticles; particleIndex++) {
        for(int column=0; column<numColumns; column++) {
            int index = numColumns*particleIndex + column+1; // first float was number of columns
            if(index >= numFloats) {
                qDebug() << "XYZBinaryReader::readFile index out of bounce.";
                qDebug() << "Column " << column << " and particle index " << particleIndex << " gives index " << index << " (max " << numColumns*m_numParticles << ")";
                exit(0);
            }
            float value = values[index];
            m_columns[column][particleIndex] = value;
        }
    }
    bytes.clear();
    return true;
}

int XYZBinaryReader::numParticles() const
{
    return m_numParticles;
}

const QVector<QVector<float>> &XYZBinaryReader::columns() const
{
    return m_columns;
}
