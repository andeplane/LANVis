#ifndef STATE_H
#define STATE_H

#include <QObject>
#include <QJsonObject>
#include "particles.h"

class State : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int timestamp READ timestamp WRITE setTimestamp NOTIFY timestampChanged)
    Q_PROPERTY(Particles* particles READ particles WRITE setParticles NOTIFY particlesChanged)
public:
    explicit State(QObject *parent = 0);
    int timestamp() const;
    Particles* particles() const;
    void update(const QJsonObject &object);
    Q_INVOKABLE void writeXYZ(QString fileName);

signals:
    void timestampChanged(int timestamp);
    void particlesChanged(Particles* particles);

public slots:
    void setTimestamp(int timestamp);
    void setParticles(Particles* particles);

private:
    double m_timestamp;
    Particles* m_particles;
};

#endif // STATE_H
