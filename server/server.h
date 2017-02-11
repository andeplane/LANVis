#ifndef SERVER_H
#define SERVER_H
#include <QVector>
#include <QString>
#include <QMap>
#include <vector>
#include "particlestyle.h"
#include "chunk.h"
#include "state.h"
#include "../client/particle.h"

class Server
{
public:
    Server();
    void loadXYZ(QString fileName);
    void loadXYZBinary(QString fileName);
    void loadLAMMPSBinary(QString fileName);
    void loadLAMMPSTextDump(QString fileName);
    bool update(QString clientStateFileName);
    void updatePositions();
    void writePositions();
    void writeState();
    const std::vector<Particle> &particles() const;
    QString dataFileName() const;
    void setDataFileName(const QString &dataFileName);

    QString stateFileName() const;
    void setStateFileName(const QString &stateFileName);

    QString lockFileName() const;
    void setLockFileName(const QString &lockFileName);
    State *currentState() const;

private:
    int m_maxNumberOfParticles;
    float m_chunkSize;
    float m_lodDistance;
    int m_lodLevels;
    bool m_sort;
    std::vector<Particle> m_particles;
    QString m_fileName;
    QString m_dataFileName;
    QString m_lockFileName;
    QString m_stateFileName;
    QVector3D m_cameraPosition;
    QVector<State> m_states;
    QMap<QString, ParticleStyle*> m_particleStyles;
    State *m_currentState;

    void setDefaultStyles();
};

#endif // SERVER_H
