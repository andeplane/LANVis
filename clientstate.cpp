#include "clientstate.h"

ClientState::ClientState(QObject *parent) : QObject(parent)
{

}

QVector3D ClientState::cameraPosition() const
{
    return m_cameraPosition;
}

void ClientState::setCameraPosition(QVector3D cameraPosition)
{
    if (m_cameraPosition == cameraPosition)
        return;

    m_cameraPosition = cameraPosition;
    emit cameraPositionChanged(cameraPosition);
}
