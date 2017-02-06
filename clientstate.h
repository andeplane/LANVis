#ifndef CLIENTSTATE_H
#define CLIENTSTATE_H

#include <QObject>
#include <QVector3D>

class ClientState : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D cameraPosition READ cameraPosition WRITE setCameraPosition NOTIFY cameraPositionChanged)

public:
    explicit ClientState(QObject *parent = 0);
    QVector3D cameraPosition() const;

signals:

    void cameraPositionChanged(QVector3D cameraPosition);

public slots:
    void setCameraPosition(QVector3D cameraPosition);

private:
    QVector3D m_cameraPosition;
};

#endif // CLIENTSTATE_H
