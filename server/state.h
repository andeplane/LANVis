#ifndef STATE_H
#define STATE_H

#include <QObject>
#include "chunk.h"
#include "generalparticle.h"

class State : public QObject
{
    Q_OBJECT
public:
    explicit State(QObject *parent = 0);
    void placeParticlesInChunks();
    void updatePositions();
    const std::vector<Particle> &particles() const;
    const std::vector<Particle> &allParticles() const;
    void writePositions();
    void writeState();
signals:

public slots:

private:
    inline int index(const int &i, const int &j, const int &k) { return i*m_ny*m_nz + j*m_nz + k; }
    void sortChunks();
    QVector3D m_boundingBoxMin;
    QVector3D m_boundingBoxMax;
    QVector3D m_origo;
    QVector3D m_size;
    int m_nx, m_ny, m_nz;
    QVector<GeneralParticle> m_allParticles;
    std::vector<Chunk>    m_chunks;
    std::vector<Chunk*>   m_chunkPtrs;
};

#endif // STATE_H
