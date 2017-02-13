#ifndef SERVER_H
#define SERVER_H
#include <QVector>
#include <QString>
#include <QMap>
#include <vector>
#include "chunk.h"
#include "../client/clientstate.h"
#include "state.h"
#include "particlesubset.h"

class Server
{
public:
    Server();
    void loadXYZ(QString fileName);
    void loadXYZBinary(QString fileName);
    void loadLAMMPSBinary(QString fileName);
    void loadLAMMPSTextDump(QString fileName);
    void writePositions();
    bool update();

    QString dataFileName() const;
    void setDataFileName(const QString &dataFileName);

    QString stateFileName() const;
    void setStateFileName(const QString &stateFileName);

    QString lockFileName() const;
    void setLockFileName(const QString &lockFileName);

private:
    ClientState m_clientState;
    QString m_fileName;
    QString m_dataFileName;
    QString m_lockFileName;
    QString m_stateFileName;

    QVector<State*> m_states;
    State *m_currentState;
    ParticleSubset m_subset;
    void setDefaultStyles();
    void setupChunks();
};

#endif // SERVER_H
