#ifndef SERVER_H
#define SERVER_H
#include <QVector>
#include <QString>
#include <QMap>
#include "atomstyle.h"
#include "particle.h"
class Server
{
public:
    Server();
    void loadXYZ(QString fileName);
    void update(QString clientStateFileName);
    void updatePositions();
    void writePositions(QString fileName);
private:
    QString m_fileName;
    float m_rCut;
    QVector3D m_cameraPosition;
    QVector<Particle> m_allParticles;
    QVector<Particle> m_subParticles;
    QMap<QString, AtomStyle*> m_atomStyles;
    void setDefaultStyles();
};

#endif // SERVER_H
