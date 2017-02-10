#include "mysimulator.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <random>
#include <SimVis/SphereData>

MyWorker::MyWorker() : m_clientState(nullptr), m_state(nullptr)
{

}

MySimulator::MySimulator(QNode *parent)
    : Simulator(parent), m_state(new State(this)), m_clientState(new ClientState(this))

{
    m_state->particles()->setSphereData(new SphereData(this));

    m_state->connect(m_state, &State::didWriteXYZFile, [&](QString fileName) {
        emit message(QString("Saved XYZ file to %1").arg(fileName));
        emit message(QString("Columns: x y z radius red green blue"));
    });
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

ClientState *MySimulator::clientState() const
{
    return m_clientState;
}

QVector3D MySimulator::cameraPosition() const
{
    return m_cameraPosition;
}

QString MySimulator::clientStateFileName() const
{
    return m_clientStateFileName;
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

void MySimulator::setClientState(ClientState *clientState)
{
    if (m_clientState == clientState)
        return;

    m_clientState = clientState;
    emit clientStateChanged(clientState);
}

void MySimulator::setCameraPosition(QVector3D cameraPosition)
{
    if (m_cameraPosition == cameraPosition)
        return;

    m_cameraPosition = cameraPosition;
    emit cameraPositionChanged(cameraPosition);
}

void MySimulator::setClientStateFileName(QString clientStateFileName)
{
    if (m_clientStateFileName == clientStateFileName)
        return;

    m_clientStateFileName = clientStateFileName;
    emit clientStateFileNameChanged(clientStateFileName);
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
        m_clientState = mySimulator->clientState();
        m_clientStateFileName = mySimulator->clientStateFileName();
        m_stateFileName = mySimulator->stateFileName();
        m_typesFileName = mySimulator->typesFileName();
        m_state->particles()->synchronizeRenderer();
        mySimulator->clientState()->setCameraPosition(mySimulator->cameraPosition());
    }
}

void MyWorker::work()
{
    if(m_state && !m_stateFileName.isEmpty()) {
        QFile loadFile(m_stateFileName);
        if (!loadFile.open(QIODevice::ReadOnly)) {
            qWarning("Couldn't open state file");
            return;
        }

        QByteArray stateData = loadFile.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(stateData));
        m_state->update(loadDoc.object());
    }
    if(m_clientState) m_clientState->save(m_clientStateFileName);
}
