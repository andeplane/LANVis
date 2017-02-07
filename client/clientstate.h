#ifndef CLIENTSTATE_H
#define CLIENTSTATE_H

#include <QObject>
#include <QVector3D>

class ClientState : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D cameraPosition READ cameraPosition WRITE setCameraPosition NOTIFY cameraPositionChanged)
    Q_PROPERTY(int maxNumberOfAtoms READ maxNumberOfAtoms WRITE setMaxNumberOfAtoms NOTIFY maxNumberOfAtomsChanged)
public:
    explicit ClientState(QObject *parent = 0);
    QVector3D cameraPosition() const;
    void save(QString fileName);
    int maxNumberOfAtoms() const;

signals:
    void cameraPositionChanged(QVector3D cameraPosition);
    void maxNumberOfAtomsChanged(int maxNumberOfAtoms);

public slots:
    void setCameraPosition(QVector3D cameraPosition);
    void setMaxNumberOfAtoms(int maxNumberOfAtoms);

private:
    QVector3D m_cameraPosition;
    int m_maxNumberOfAtoms;
};

#endif // CLIENTSTATE_H
