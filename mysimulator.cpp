#include "mysimulator.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <random>
#include <SimVis/SphereData>

MyWorker::MyWorker() : m_state(nullptr)
{

}

MySimulator::MySimulator(QNode *parent)
    : Simulator(parent), m_state(new State())

{

}

State *MySimulator::state() const
{
    return m_state;
}

QString MySimulator::stateFileName() const
{
    return m_stateFileName;
}

QString MySimulator::typesFileName() const
{
    return m_typesFileName;
}

void MySimulator::setState(State *state)
{
    if (m_state == state)
        return;

    m_state = state;
    emit stateChanged(state);
}

void MySimulator::setStateFileName(QString stateFileName)
{
    if (m_stateFileName == stateFileName)
        return;

    m_stateFileName = stateFileName;
    emit stateFileNameChanged(stateFileName);
}

void MySimulator::setTypesFileName(QString typesFileName)
{
    if (m_typesFileName == typesFileName)
        return;

    m_typesFileName = typesFileName;
    emit typesFileNameChanged(typesFileName);
}

SimulatorWorker *MySimulator::createWorker()
{
    return new MyWorker();
}

void MyWorker::synchronizeSimulator(Simulator *simulator)
{
    MySimulator *mySimulator = qobject_cast<MySimulator*>(simulator);
    if(mySimulator) {
        m_state = mySimulator->state();
        m_state->atoms()->synchronizeRenderer();
        m_stateFileName = mySimulator->stateFileName();

        // Synchronize data between QML thread and computing thread (MySimulator is on QML, MyWorker is computing thread).
        // This is for instance data from user through GUI (sliders, buttons, text fields etc)
    }
}

void MyWorker::work()
{
    if(m_state) {
        qDebug() << "Trying to open " << m_stateFileName;
        QFile loadFile(m_stateFileName);
        if (!loadFile.open(QIODevice::ReadOnly)) {
            qWarning("Couldn't open state file");
            return;
        }

        QByteArray stateData = loadFile.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(stateData));
        m_state->update(loadDoc.object());
    }
}
