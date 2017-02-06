#ifndef MYSIMULATOR_H
#define MYSIMULATOR_H
#include <SimVis/Simulator>
#include <QVector>
#include <QVector3D>
#include <QElapsedTimer>
#include <Qt3DRender/QBuffer>
#include <SimVis/SphereData>

class MyWorker : public SimulatorWorker
{
    Q_OBJECT
public:
    MyWorker();

private:
    // SimulatorWorker interface
    virtual void synchronizeSimulator(Simulator *simulator);
    virtual void work();
    QVector<QVector3D> m_positions;
};

class MySimulator : public Simulator
{
    Q_OBJECT
    Q_PROPERTY(SphereData* sphereData READ sphereData CONSTANT)

public:
    MySimulator(QNode *parent = 0);

    SphereData* sphereData();

public slots:

signals:

protected:
    virtual SimulatorWorker *createWorker() override;

private:
    QScopedPointer<SphereData> m_sphereData;
};

#endif // MYSIMULATOR_H
