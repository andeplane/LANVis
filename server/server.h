#ifndef SERVER_H
#define SERVER_H
#include <QVector>
#include <QString>
#include <QMap>
#include <vector>
#include "chunk.h"
#include "../client/clientstate.h"
#include "../client/serversettings.h"
#include "state.h"
#include "particlesubset.h"

class Server
{
public:
    Server();
    bool loadXYZ(QString fileName);
    void loadXYZBinary(QString fileName);
    void loadLAMMPSBinary(QString fileName);
    void loadLAMMPSTextDump(QString fileName);
    void writePositions();
    bool update();
    void save();

    QString lockFileName() const;
    void setLockFileName(const QString &lockFileName);
    State *currentState() const;
    const ParticleSubset &subset() const;

private:
    void reset();
    ClientState m_clientState;
    QString m_lockFileName;

    QVector<State*> m_states;
    State *m_currentState;
    ParticleSubset m_subset;

    ServerSettings m_settings;
    bool loadFile();
};

#endif // SERVER_H
