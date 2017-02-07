#ifndef SERVER_H
#define SERVER_H
#include <QVector>
#include <QString>
#include <QMap>
#include "atomstyle.h"
#include "chunk.h"
#include "../client/particle.h"

class Server
{
public:
    Server();
    void loadXYZ(QString fileName);
    void loadLAMMPSBinary(QString fileName);
    void update(QString clientStateFileName);
    void updatePositions();
    void writePositions();
    void writeState();
    QString dataFileName() const;
    void setDataFileName(const QString &dataFileName);

    QString stateFileName() const;
    void setStateFileName(const QString &stateFileName);
    const QVector<Particle> &particles() const;

private:
    inline int index(const int &i, const int &j, const int &k) { return i*m_ny*m_nz + j*m_nz + k; }
    QVector3D m_origo;
    QVector3D m_size;
    int m_nx, m_ny, m_nz;
    float m_rCut;
    float m_chunkSize;
    QString m_fileName;
    QString m_dataFileName;
    QString m_stateFileName;
    QVector3D m_cameraPosition;
    QVector<Chunk>    m_chunks;
    QVector<Particle> m_particles;
    QVector<Particle> m_allParticles;
    QMap<QString, AtomStyle*> m_atomStyles;
    void setDefaultStyles();
    void setupChunks();
    void placeParticleInChunks();
};

#endif // SERVER_H
