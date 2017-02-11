#ifndef SERVER_H
#define SERVER_H
#include <QVector>
#include <QString>
#include <QMap>
#include <vector>
#include "particlestyle.h"
#include "chunk.h"
#include "../client/clientstate.h"

class Server
{
public:
    Server();
    void loadXYZ(QString fileName);
    void loadXYZBinary(QString fileName);
    void loadLAMMPSBinary(QString fileName);
    bool update(QString clientStateFileName);

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
    QVector3D m_cameraPosition;
    QMap<QString, ParticleStyle*> m_particleStyles;
    void setDefaultStyles();
    void setupChunks();
};

#endif // SERVER_H
