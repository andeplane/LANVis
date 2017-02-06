#ifndef MYSIMULATOR_H
#define MYSIMULATOR_H
#include <SimVis/Simulator>
#include <QVector>
#include <QVector3D>
#include <QElapsedTimer>
#include <Qt3DRender/QBuffer>
#include <SimVis/SphereData>
#include "state.h"

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
    State *m_state;
    QString m_stateFileName;
};

class MySimulator : public Simulator
{
    Q_OBJECT
    Q_PROPERTY(State* state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(QString stateFileName READ stateFileName WRITE setStateFileName NOTIFY stateFileNameChanged)
    Q_PROPERTY(QString typesFileName READ typesFileName WRITE setTypesFileName NOTIFY typesFileNameChanged)
public:
    MySimulator(QNode *parent = 0);
    State* state() const;
    QString stateFileName() const;
    QString typesFileName() const;

public slots:
    void setState(State* state);
    void setStateFileName(QString stateFileName);
    void setTypesFileName(QString typesFileName);

signals:
    void stateChanged(State* state);
    void stateFileNameChanged(QString stateFileName);
    void typesFileNameChanged(QString typesFileName);

protected:
    virtual SimulatorWorker *createWorker() override;

private:
    State* m_state;
    QString m_stateFileName;
    QString m_typesFileName;
};

#endif // MYSIMULATOR_H
