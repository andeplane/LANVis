#include "xyzbinaryimporter.h"
#include "state.h"
#include "../client/serversettings.h"

#include <QDebug>

XYZBinaryImporter::XYZBinaryImporter()
{

}

bool XYZBinaryImporter::doCheckFileFormat(QFile &file)
{

}

bool XYZBinaryImporter::doReadFile(QString fileName, QVector<State *> &states, ClientState &clientState)
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

    int numParticles = bytes.length() / sizeof(float) / numColumns;
    qDebug() << "and " << numParticles << " particles.";

    if(numColumns-4 != clientState.serverSettings()->propertyNames().size()) {
        qDebug() << "Error, number of columns does not match client state.";
        return false;
    }

    State *state = new State();
    state->setParticlePropertyNames(clientState.serverSettings()->propertyNames());
    state->setNumberOfParticles(numParticles);
    states.push_back(state);

    std::vector<float> columns(numColumns-4, 0);

    int numFloats = numColumns*numParticles+1;
    QVector3D min,max;
    for(int particleIndex=0; particleIndex<numParticles; particleIndex++) {

        // +1 in indices beacuse first float was number of columns
        int type = static_cast<int>(values[ 1+numColumns*particleIndex+0 ]);
        float x = values[ 1+numColumns*particleIndex+1 ];
        float y = values[ 1+numColumns*particleIndex+2 ];
        float z = values[ 1+numColumns*particleIndex+3 ];
        min[0] = std::min(min[0], x);
        min[1] = std::min(min[1], y);
        min[2] = std::min(min[2], z);

        max[0] = std::max(max[0], x);
        max[1] = std::max(max[1], y);
        max[2] = std::max(max[2], z);

        for(int column=4; column<numColumns; column++) {
            int index = numColumns*particleIndex + column+1;
            if(index >= numFloats) {
                qDebug() << "XYZBinaryReader::readFile index out of bounce.";
                qDebug() << "Column " << column << " and particle index " << particleIndex << " gives index " << index << " (max " << numColumns*numParticles << ")";
                return false;
            }
            columns[column-4] = values[index];
        }
        state->addParticle(QVector3D(x,y,z), QString("%1").arg(type), columns);
    }
    QVector3D origo = min;
    QVector3D size = max-min;
    state->setOrigo(origo);
    state->setSize(size);
    state->applyModifiers(clientState);
    state->placeParticlesInChunks(clientState);
    bytes.clear();
    return true;
}
