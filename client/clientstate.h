#ifndef CLIENTSTATE_H
#define CLIENTSTATE_H

#include <QObject>
#include <QVector3D>

class ClientState : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D cameraPosition READ cameraPosition WRITE setCameraPosition NOTIFY cameraPositionChanged)
    Q_PROPERTY(int maxNumberOfAtoms READ maxNumberOfAtoms WRITE setMaxNumberOfAtoms NOTIFY maxNumberOfAtomsChanged)
    Q_PROPERTY(bool sort READ sort WRITE setSort NOTIFY sortChanged)
public:
    explicit ClientState(QObject *parent = 0);
    QVector3D cameraPosition() const;
    void save(QString fileName);
    int maxNumberOfAtoms() const;
    bool sort() const;

signals:
    void cameraPositionChanged(QVector3D cameraPosition);
    void maxNumberOfAtomsChanged(int maxNumberOfAtoms);
    void sortChanged(bool sort);

public slots:
    void setCameraPosition(QVector3D cameraPosition);
    void setMaxNumberOfAtoms(int maxNumberOfAtoms);
    void setSort(bool sort);

private:
    QVector3D m_cameraPosition;
    int m_maxNumberOfAtoms;
    bool m_sort;
};

#endif // CLIENTSTATE_H
