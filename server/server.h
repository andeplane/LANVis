#ifndef SERVER_H
#define SERVER_H
#include <QVector>
#include <QString>
#include "particle.h"
class Server
{
public:
    Server();

private:
    QString m_fileName;
    float m_rCut;
    QVector3D m_cameraPosition;
    QVector<Particle> m_particles;
};

#endif // SERVER_H
