#ifndef STATE_H
#define STATE_H

#include <QMap>
#include <QObject>
#include <vector>
#include "chunk.h"

class State : public QObject
{
    Q_OBJECT
public:
    explicit State(QObject *parent = 0);
    State(const State &state);
    ~State();
    const std::vector<Particle> &allParticles() const;
    void sortChunks(QVector3D point);
    void placeParticlesInChunks(float chunkSize, int lodLevels);
    void addParticles(const std::vector<QVector3D> &positions, const std::vector<int> types, QVector3D origo, QVector3D size);

    void reset();
    QVector3D boundingBoxMin() const;
    void setBoundingBoxMin(const QVector3D &boundingBoxMin);
    QVector3D boundingBoxMax() const;
    void setBoundingBoxMax(const QVector3D &boundingBoxMax);
    std::vector<Chunk *> chunkPtrs() const;
    void setParticleStyles(const QMap<QString, struct ParticleStyle *> &particleStyles);

signals:

public slots:

private:
    QMap<QString, struct ParticleStyle*> m_particleStyles;
    inline int index(const int &i, const int &j, const int &k) { return i*m_ny*m_nz + j*m_nz + k; }
    int m_nx, m_ny, m_nz;
    float m_chunkSize;
    QVector3D m_boundingBoxMin;
    QVector3D m_boundingBoxMax;
    QVector3D m_origo;
    QVector3D m_size;
    std::vector<Chunk>    m_chunks;
    std::vector<Chunk*>   m_chunkPtrs;
    std::vector<Particle> m_allParticles;
    void setupChunks();
};

#endif // STATE_H
