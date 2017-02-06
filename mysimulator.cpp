#include "mysimulator.h"
#include <QDebug>
#include <random>
#include <SimVis/SphereData>
MySimulator::MySimulator(QNode *parent)
    : Simulator(parent)
    , m_sphereData(new SphereData(this))
{
}

SphereData *MySimulator::sphereData()
{
    return m_sphereData.data();
}

SimulatorWorker *MySimulator::createWorker()
{
    return new MyWorker();
}

MyWorker::MyWorker()
{
    std::random_device rd;
    std::default_random_engine re(rd());
    std::uniform_real_distribution<float> nextFloat(-100, 100);

    m_positions.resize(1000);
    for(QVector3D &position : m_positions) {
        position[0] = nextFloat(re);
        position[1] = nextFloat(re);
        position[2] = nextFloat(re);
    }
}

void MyWorker::synchronizeSimulator(Simulator *simulator)
{
    MySimulator *mySimulator = qobject_cast<MySimulator*>(simulator);
    if(mySimulator) {
        // Synchronize data between QML thread and computing thread (MySimulator is on QML, MyWorker is computing thread).
        // This is for instance data from user through GUI (sliders, buttons, text fields etc)
        mySimulator->sphereData()->setPositions(m_positions);
    }
}

void MyWorker::work()
{

}
