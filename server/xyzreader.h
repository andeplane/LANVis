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
    bool readFile(QString filename);
    const std::vector<QString> &types() const;
    const std::vector<QVector3D> &positions() const;
    QVector3D origo() const;
    QVector3D size() const;

signals:

public slots:

private:
    QVector3D m_origo;
    QVector3D m_size;
    std::vector<QString>   m_types;
    std::vector<QVector3D> m_positions;
};

#endif // XYZREADER_H
