#ifndef XYZBINARYREADER_H
#define XYZBINARYREADER_H

#include <QObject>
#include <QVector>



class XYZBinaryReader : public QObject
{
    Q_OBJECT
public:
    explicit XYZBinaryReader(QObject *parent = nullptr);
    bool readFile(QString fileName);
    int numParticles() const;

    const QVector<QVector<float>> &columns() const;

private:
    int m_numParticles;
    QVector<QVector<float>> m_columns;
};

#endif // XYZBINARYREADER_H
