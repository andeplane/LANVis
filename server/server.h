#ifndef SERVER_H
#define SERVER_H
#include <QVector>
#include <QString>
#include <QMap>
#include "atomstyle.h"
#include "../client/particle.h"

class Server
{
public:
    Server();
    void loadXYZ(QString fileName);
    void update(QString clientStateFileName);
    void updatePositions();
    void writePositions();
    void writeState();
    QString dataFileName() const;
    void setDataFileName(const QString &dataFileName);

    QString stateFileName() const;
    void setStateFileName(const QString &stateFileName);

private:
    QString m_fileName;
    QString m_dataFileName;
    QString m_stateFileName;
    float m_rCut;
    QVector3D m_cameraPosition;
    QVector<Particle> m_allParticles;
    QVector<Particle> m_subParticles;
    QMap<QString, AtomStyle*> m_atomStyles;
    void setDefaultStyles();
};

#endif // SERVER_H
