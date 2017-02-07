#ifndef CLIENTSTATE_H
#define CLIENTSTATE_H

#include <QObject>
#include <QVector3D>

class ClientState : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D cameraPosition READ cameraPosition WRITE setCameraPosition NOTIFY cameraPositionChanged)
    Q_PROPERTY(float rCut READ rCut WRITE setRCut NOTIFY rCutChanged)
public:
    explicit ClientState(QObject *parent = 0);
    QVector3D cameraPosition() const;
    void save(QString fileName);
    float rCut() const;

signals:
    void cameraPositionChanged(QVector3D cameraPosition);
    void rCutChanged(float rCut);

public slots:
    void setCameraPosition(QVector3D cameraPosition);
    void setRCut(float rCut);

private:
    QVector3D m_cameraPosition;
    float m_rCut;
};

#endif // CLIENTSTATE_H
