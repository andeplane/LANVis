#ifndef CHUNK_H
#define CHUNK_H

#include "../client/particle.h"
#include "../client/clientstate.h"
#include <QVector>
#include <vector>
#include <random>

class Chunk
{
public:
    Chunk();
//    Chunk(Chunk const &) = delete;
//    void operator=(Chunk const &) = delete;
    // ~Chunk() { m_corners.clear(); m_particles.clear(); }
    void reset();
    float minDistanceTo(const QVector3D &point) const;
    QVector3D nearestCorner(const QVector3D &point) const;
    void sort(const QVector3D &point, const std::vector<IdentifiableParticle> &particles, int lod);
    std::vector<QVector3D> &corners();
    std::vector<int> &particleIndices(int lod);
    void buildLOD(ClientState &clientState, std::mt19937 &generator, std::uniform_real_distribution<float> &distribution);

private:
    std::vector<QVector3D> m_corners;
    std::vector<std::vector<int>> m_particleIndices;
};

#endif // CHUNK_H
