#ifndef STATE_H
#define STATE_H

#include <QObject>
#include <vector>
#include "chunk.h"
#include "../client/particle.h"
#include "../client/clientstate.h"

class State : public QObject
{
    Q_OBJECT
public:
    explicit State(QObject *parent = 0);
    State(const State &state);
    ~State();
    const std::vector<IdentifiableParticle> &allParticles() const;
    void sortChunks(QVector3D point);
    void placeParticlesInChunks(ClientState &clientState);
    void addParticles(const std::vector<QVector3D> &positions, const std::vector<QString> types, QVector3D origo, QVector3D size);
    void addParticle(QVector3D position, QString type, std::vector<float> particleProperties = {});
    void setNumberOfParticles(int numberOfParticles);
    void reset();
    void save();
    std::vector<Chunk *> chunkPtrs() const;
    void setParticleStyles(const QMap<QString, struct ParticleStyle *> &particleStyles);
    void setParticlePropertyNames(QStringList particlePropertyNames);
    QVector3D origo() const;
    void setOrigo(const QVector3D &origo);
    QVector3D size() const;
    void setSize(const QVector3D &size);
    QMap<QString, ParticleStyle *> &particleStyles();
    void applyModifiers(ClientState &state);
signals:

public slots:

private:
    inline size_t index(const int &i, const int &j, const int &k) { return i*m_ny*m_nz + j*m_nz + k; }
    int m_nx, m_ny, m_nz;
    float m_chunkSize;
    QVector3D m_origo;
    QVector3D m_size;
    std::vector<Chunk>    m_chunks;
    std::vector<Chunk*>   m_chunkPtrs;
    std::vector<IdentifiableParticle> m_allParticles;
    std::vector<bool> m_selected;
    std::vector<int> m_particleIndices;
    std::vector<std::vector<float>> m_particleProperties;
    QStringList m_particlePropertyNames;
    void setupChunks();
};

#endif // STATE_H
