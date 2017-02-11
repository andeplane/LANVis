#ifndef CLIENTSTATE_H
#define CLIENTSTATE_H

#include <QObject>
#include <QVector3D>

class ClientState : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D cameraPosition READ cameraPosition WRITE setCameraPosition NOTIFY cameraPositionChanged)
    Q_PROPERTY(int maxNumberOfParticles READ maxNumberOfParticles WRITE setMaxNumberOfParticles NOTIFY maxNumberOfParticlesChanged)
    Q_PROPERTY(bool sort READ sort WRITE setSort NOTIFY sortChanged)
    Q_PROPERTY(float chunkSize READ chunkSize WRITE setChunkSize NOTIFY chunkSizeChanged)
    Q_PROPERTY(float lodDistance READ lodDistance WRITE setLodDistance NOTIFY lodDistanceChanged)
    Q_PROPERTY(int lodLevels READ lodLevels WRITE setLodLevels NOTIFY lodLevelsChanged)
public:
    explicit ClientState(QObject *parent = 0);
    QVector3D cameraPosition() const;
    void save(QString fileName);
    void load(QString fileName);
    int maxNumberOfParticles() const;
    bool sort() const;
    float chunkSize() const;
    float lodDistance() const;
    int lodLevels() const;
    bool chunksDirty() const;
    bool particlesDirty() const;
    void setChunksDirty(bool chunksDirty);
    void setParticlesDirty(bool particlesDirty);

signals:
    void cameraPositionChanged(QVector3D cameraPosition);
    void maxNumberOfParticlesChanged(int maxNumberOfParticles);
    void sortChanged(bool sort);
    void chunkSizeChanged(float chunkSize);
    void lodDistanceChanged(float lodDistance);
    void lodLevelsChanged(int lodLevels);

public slots:
    void setCameraPosition(QVector3D cameraPosition);
    void setMaxNumberOfParticles(int maxNumberOfParticles);
    void setSort(bool sort);
    void setChunkSize(float chunkSize);
    void setLodDistance(float lodDistance);
    void setLodLevels(int lodLevels);

private:
    bool m_chunksDirty;
    bool m_particlesDirty;
    QVector3D m_cameraPosition;
    int m_maxNumberOfParticles;
    bool m_sort;
    float m_chunkSize;
    float m_lodDistance;
    int m_lodLevels;
};

#endif // CLIENTSTATE_H
