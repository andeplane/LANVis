#ifndef STATESUBSET_H
#define STATESUBSET_H
#include "../client/particle.h"
#include "state.h"
#include <QObject>
#include <vector>


class StateSubset : public QObject
{
    Q_OBJECT
public:
    explicit StateSubset(QObject *parent = 0);
    std::vector<Particle> &particles();
    QVector3D boundingBoxMin() const;
    QVector3D boundingBoxMax() const;
    void updatePositions(State &state, QVector3D cameraPosition, float lodDistance, int lodLevels, int maxNumberOfParticles, bool sort);

signals:

public slots:

private:
    State *m_state;
    QVector3D m_boundingBoxMin;
    QVector3D m_boundingBoxMax;
    std::vector<Particle> m_particles;
};

#endif // STATESUBSET_H
