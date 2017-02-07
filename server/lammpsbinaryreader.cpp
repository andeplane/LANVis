#include "lammpsbinaryreader.h"
#include <QDebug>
const QVector<QVector3D> &LAMMPSBinaryReader::positions() const
{
    return m_positions;
}

QVector<int> LAMMPSBinaryReader::types() const
{
    return m_types;
}

QVector3D LAMMPSBinaryReader::origo() const
{
    return m_origo;
}

QVector3D LAMMPSBinaryReader::size() const
{
    return m_size;
}

void LAMMPSBinaryReader::readFile(QString fileName)
{
    FILE *filePtr = fopen(fileName.toLocal8Bit().constData(),"rb");
    if (!filePtr) {
        qDebug() << "ERROR: Could not open file " << fileName;
        return;
    }

    int maxbuf = 0;
    double *buf = NULL;
    State state;

    fread(&state.timestep,sizeof(bigint),1,filePtr);
    // detect end-of-file
    if (feof(filePtr)) {
        fclose(filePtr);
        return;
    }

    bigint natoms;
    int size_one, nchunk;
    int boundary[3][2];
    char boundstr[9];

    fread(&natoms,sizeof(bigint),1,filePtr);
    m_positions.resize(natoms);
    m_types.resize(natoms);
    fread(&state.triclinic,sizeof(int),1,filePtr);

    fread(&state.boundary[0][0],6*sizeof(int),1,filePtr);
    fread(&state.xlo,sizeof(double),1,filePtr);
    fread(&state.xhi,sizeof(double),1,filePtr);
    fread(&state.ylo,sizeof(double),1,filePtr);
    fread(&state.yhi,sizeof(double),1,filePtr);
    fread(&state.zlo,sizeof(double),1,filePtr);
    fread(&state.zhi,sizeof(double),1,filePtr);
    if (state.triclinic) {
        fread(&state.xy,sizeof(double),1,filePtr);
        fread(&state.xz,sizeof(double),1,filePtr);
        fread(&state.yz,sizeof(double),1,filePtr);
    }
    m_origo[0] = state.xlo; m_origo[1] = state.ylo; m_origo[2] = state.zlo;
    m_size[0] = state.xhi-state.xlo; m_size[1] = state.yhi-state.ylo; m_size[2] = state.zhi-state.zlo;

    fread(&size_one,sizeof(int),1,filePtr);
    fread(&nchunk,sizeof(int),1,filePtr);
    state.atoms.resize(natoms);
    int atomIndex = 0;
    // loop over processor chunks in file
    for (int i = 0; i < nchunk; i++) {
        int chunkSize;
        fread(&chunkSize,sizeof(int),1,filePtr);
        // extend buffer to fit chunk size

        if (chunkSize > maxbuf) {
            if (buf) delete [] buf;
            buf = new double[chunkSize];
            maxbuf = chunkSize;
        }

        // read chunk and write as size_one values per line

        fread(buf,sizeof(double),chunkSize,filePtr);
        int numAtomsInChunk = chunkSize /= size_one;
        int m = 0;
        for (int chunk = 0; chunk < numAtomsInChunk; chunk++) {
            int atomId = buf[m++];
            Q_UNUSED(atomId)
            m_types[atomIndex] = buf[m++];
            m_positions[atomIndex][0] = m_origo[0] + buf[m++]*m_size[0];
            m_positions[atomIndex][1] = m_origo[1] + buf[m++]*m_size[1];
            m_positions[atomIndex][2] = m_origo[2] + buf[m++]*m_size[2];

            atomIndex++;
            if(size_one > 5) {
                qDebug() << "Error, this binary file has more columns than we support";
                exit(1);
            }
        }
    }
    if(buf) delete [] buf;
    fclose(filePtr);
}
