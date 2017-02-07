#ifndef STATE_H
#define STATE_H

#include <QObject>
#include <QJsonObject>
#include "atoms.h"

class State : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int timestamp READ timestamp WRITE setTimestamp NOTIFY timestampChanged)
    Q_PROPERTY(Atoms* atoms READ atoms WRITE setAtoms NOTIFY atomsChanged)
public:
    explicit State(QObject *parent = 0);
    int timestamp() const;
    Atoms* atoms() const;
    void update(const QJsonObject &object);
signals:
    void timestampChanged(int timestamp);
    void atomsChanged(Atoms* atoms);

public slots:
    void setTimestamp(int timestamp);
    void setAtoms(Atoms* atoms);

private:
    double m_timestamp;
    Atoms* m_atoms;
};

#endif // STATE_H
