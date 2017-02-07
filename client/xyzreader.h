#ifndef XYZREADER_H
#define XYZREADER_H
#include <QString>
#include <QObject>
#include <QVector>
#include <QVector3D>

class XYZReader : public QObject
{
    Q_OBJECT
public:
    explicit XYZReader(QObject *parent = 0);
    void readFile(QString filename);
    const QVector<QString> &types() const;
    void setTypes(const QVector<QString> &types);

    const QVector<QVector3D> &positions() const;
    void setPositions(const QVector<QVector3D> &positions);

signals:

public slots:

private:
    QVector<QString>   m_types;
    QVector<QVector3D> m_positions;

};

#endif // XYZREADER_H
