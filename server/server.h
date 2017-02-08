#ifndef SERVER_H
#define SERVER_H
#include <QVector>
#include <QString>
#include <QMap>
#include <vector>
#include "particlestyle.h"
#include "chunk.h"
#include "../client/particle.h"

class Server
{
public:
    Server();
    void loadXYZ(QString fileName);
    void loadLAMMPSBinary(QString fileName);
    bool update(QString clientStateFileName);
    void updatePositions();
    void writePositions();
    void writeState();
    QString dataFileName() const;
    void setDataFileName(const QString &dataFileName);

    QString stateFileName() const;
    void setStateFileName(const QString &stateFileName);
    const std::vector<Particle> &particles() const;
    const std::vector<Particle> &allParticles() const;

    QString lockFileName() const;
    void setLockFileName(const QString &lockFileName);
private:
    void sortChunks();
    inline int index(const int &i, const int &j, const int &k) { return i*m_ny*m_nz + j*m_nz + k; }
    QVector3D m_boundingBoxMin;
    QVector3D m_boundingBoxMax;
    QVector3D m_origo;
    QVector3D m_size;
    int m_nx, m_ny, m_nz;
    int m_maxNumberOfParticles;
    float m_chunkSize;
    float m_lodDistance;
    int m_lodLevels;
    bool m_sort;
    QString m_fileName;
    QString m_dataFileName;
    QString m_lockFileName;
    QString m_stateFileName;
    QVector3D m_cameraPosition;
    std::vector<Chunk>    m_chunks;
    std::vector<Chunk*>   m_chunkPtrs;
    std::vector<Particle> m_particles;
    std::vector<Particle> m_allParticles;
    QMap<QString, ParticleStyle*> m_particleStyles;
    void setDefaultStyles();
    void setupChunks();
    void placeParticleInChunks();
};

#endif // SERVER_H
