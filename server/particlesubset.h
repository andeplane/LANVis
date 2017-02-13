#ifndef PARTICLESUBSET_H
#define PARTICLESUBSET_H
#include <QVector3D>
#include "../client/particle.h"
#include "../client/clientstate.h"
#include "state.h"

class ParticleSubset : public QObject
{
    Q_OBJECT
public:
    explicit ParticleSubset(QObject *parent = 0);
    std::vector<ColoredParticle> &particles();
    QVector3D boundingBoxMin() const;
    QVector3D boundingBoxMax() const;
    void updatePositions(State &state, const ClientState &clientState);

signals:

public slots:

private:
    State *m_state;
    QVector3D m_boundingBoxMin;
    QVector3D m_boundingBoxMax;
    std::vector<ColoredParticle> m_particles;
    std::vector<int> m_particleIndices;
};

#endif // PARTICLESUBSET_H
