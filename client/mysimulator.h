#ifndef MYSIMULATOR_H
#define MYSIMULATOR_H
#include <SimVis/Simulator>
#include <QVector>
#include <QVector3D>
#include <QElapsedTimer>
#include <Qt3DRender/QBuffer>
#include <SimVis/SphereData>
#include "state.h"
#include "clientstate.h"

class MyWorker : public SimulatorWorker
{
    Q_OBJECT
public:
    MyWorker();

public slots:

signals:

private:
    // SimulatorWorker interface
    virtual void synchronizeSimulator(Simulator *simulator);
    virtual void work();
    QString m_clientStateFileName;
    ClientState *m_clientState;
    State *m_state;
    QString m_stateFileName;
    QString m_typesFileName;
    int m_numTimesteps = 1;
};

class MySimulator : public Simulator
{
    Q_OBJECT
    Q_PROPERTY(State* state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(ClientState* clientState READ clientState WRITE setClientState NOTIFY clientStateChanged)
    Q_PROPERTY(QString stateFileName READ stateFileName WRITE setStateFileName NOTIFY stateFileNameChanged)
    Q_PROPERTY(QString typesFileName READ typesFileName WRITE setTypesFileName NOTIFY typesFileNameChanged)
    Q_PROPERTY(QVector3D cameraPosition READ cameraPosition WRITE setCameraPosition NOTIFY cameraPositionChanged)
    Q_PROPERTY(QString clientStateFileName READ clientStateFileName WRITE setClientStateFileName NOTIFY clientStateFileNameChanged)
    Q_PROPERTY(int numTimesteps READ numTimesteps WRITE setNumTimesteps NOTIFY numTimestepsChanged)
public:
    MySimulator(QNode *parent = 0);
    State* state() const;
    QString stateFileName() const;
    QString typesFileName() const;
    ClientState* clientState() const;
    QVector3D cameraPosition() const;
    QString clientStateFileName() const;
    int numTimesteps() const;

public slots:
    void setState(State* state);
    void setStateFileName(QString stateFileName);
    void setTypesFileName(QString typesFileName);
    void setClientState(ClientState* clientState);
    void setCameraPosition(QVector3D cameraPosition);
    void setClientStateFileName(QString clientStateFileName);
    void setNumTimesteps(int numTimesteps);

signals:
    void stateChanged(State* state);
    void stateFileNameChanged(QString stateFileName);
    void typesFileNameChanged(QString typesFileName);
    void clientStateChanged(ClientState* clientState);
    void cameraPositionChanged(QVector3D cameraPosition);
    void clientStateFileNameChanged(QString clientStateFileName);
    void message(QString message);
    void numTimestepsChanged(int numTimesteps);

protected:
    virtual SimulatorWorker *createWorker() override;

private:
    State* m_state;
    QString m_stateFileName;
    QString m_typesFileName;
    QString m_clientStateFileName;
    ClientState* m_clientState;
    QVector3D m_cameraPosition;
    int m_numTimesteps;
};

#endif // MYSIMULATOR_H
