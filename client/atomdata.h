#ifndef ATOMDATA_H
#define ATOMDATA_H

#include <QObject>
#include <QVector>
#include <QVector3D>

class AtomData : public QObject
{
    Q_OBJECT
public:
    explicit AtomData(QObject *parent = 0);
    ~AtomData();
    QVector<QVector3D> positions;
    QVector<QVector3D> colors;
    QVector<float> radii;
    void resize(int size);
    int size();
    void reset();
    void sort(QVector3D cameraPosition);
    long memoryUsage();
signals:

public slots:
};

#endif // ATOMDATA_H
