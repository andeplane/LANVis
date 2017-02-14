#ifndef PARTICLESUBSET_H
#define PARTICLESUBSET_H
#include <QMap>
#include <QVector3D>
#include "../client/particle.h"
#include "../client/clientstate.h"
#include "particlestyle.h"
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
    QMap<QString, struct ParticleStyle*> m_particleStyles;
    State *m_state;
    QVector3D m_boundingBoxMin;
    QVector3D m_boundingBoxMax;
    std::vector<ColoredParticle> m_particles;
    std::vector<int> m_particleIndices;
    void setDefaultStyles();
};

#endif // PARTICLESUBSET_H
