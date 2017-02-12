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
#include "statesubset.h"

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
    std::vector<Particle> &particles();
    QString dataFileName() const;
    void setDataFileName(const QString &dataFileName);

    QString stateFileName() const;
    void setStateFileName(const QString &stateFileName);

    QString lockFileName() const;
    void setLockFileName(const QString &lockFileName);
    State *currentState() const;

    StateSubset &stateSubset();

private:
    int m_maxNumberOfParticles;
    float m_chunkSize;
    float m_lodDistance;
    int m_lodLevels;
    int m_currentStateIndex;
    bool m_sort;

    QString m_fileName;
    QString m_dataFileName;
    QString m_lockFileName;
    QString m_stateFileName;
    QVector3D m_cameraPosition;
    QVector<State*> m_states;
    State *m_currentState;
    StateSubset m_stateSubset;
};

#endif // SERVER_H
