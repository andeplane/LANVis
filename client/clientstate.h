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
    Q_PROPERTY(int numThreads READ numThreads WRITE setNumThreads NOTIFY numThreadsChanged)
    Q_PROPERTY(int timestep READ timestep WRITE setTimestep NOTIFY timestepChanged)
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
    Q_PROPERTY(ServerSettings* serverSettings READ serverSettings WRITE setServerSettings NOTIFY serverSettingsChanged)
public:
    explicit ClientState(QObject *parent = 0);
    QVector3D cameraPosition() const;
    void save();
    bool load();
    int maxNumberOfParticles() const;
    bool sort() const;
    float chunkSize() const;
    float lodDistance() const;
    int lodLevels() const;
    bool chunksDirty() const;
    bool particlesDirty() const;
    void setChunksDirty(bool chunksDirty);
    void setParticlesDirty(bool particlesDirty);
    QString fileName() const;
    bool dirty() const;
    void setDirty(bool dirty);
    class ServerSettings* serverSettings() const;
    int numThreads() const;
    int timestep() const;

signals:
    void cameraPositionChanged(QVector3D cameraPosition);
    void maxNumberOfParticlesChanged(int maxNumberOfParticles);
    void sortChanged(bool sort);
    void chunkSizeChanged(float chunkSize);
    void lodDistanceChanged(float lodDistance);
    void lodLevelsChanged(int lodLevels);
    void fileNameChanged(QString fileName);
    void serverSettingsChanged(class ServerSettings* serverSettings);
    void numThreadsChanged(int numThreads);
    void timestepChanged(int timestep);

public slots:
    void setCameraPosition(QVector3D cameraPosition);
    void setMaxNumberOfParticles(int maxNumberOfParticles);
    void setSort(bool sort);
    void setChunkSize(float chunkSize);
    void setLodDistance(float lodDistance);
    void setLodLevels(int lodLevels);
    void setFileName(QString fileName);
    void setServerSettings(class ServerSettings* serverSettings);
    void setNumThreads(int numThreads);
    void setTimestep(int timestep);

private:
    bool m_dirty;
    bool m_chunksDirty;
    bool m_particlesDirty;
    QVector3D m_cameraPosition;
    int m_maxNumberOfParticles;
    bool m_sort;
    float m_chunkSize;
    float m_lodDistance;
    int m_lodLevels;
    QString m_fileName;
    class ServerSettings* m_serverSettings;
    int m_numThreads;
    int m_timestep;
};

#endif // CLIENTSTATE_H
