#include <QElapsedTimer>
#include <QDebug>
#include <iostream>
#include <unistd.h>
#include "server.h"
// #define KIF
using namespace std;

int main(int argc, char *argv[])
{
#ifdef KIF
    QString xyzFileName("/scratch/largesystem/dump.xyz");
    QString lammpsDumpFileName("/scratch/largesystem/dump.lmp.bin");
    QString dataFileName("/scratch/largesystem/dump.bin");
    QString stateFileName("/scratch/largesystem/state.json");
    QString clientStateFileName("/scratch/largesystem/client.json");
#else
    QString xyzFileName("/projects/tmp/dump.xyz");
    QString lammpsDumpFileName("/projects/tmp/dump2.lmp.bin");
    QString dataFileName("/projects/tmp/dump.bin");
    QString stateFileName("/projects/tmp/state.json");
    QString clientStateFileName("/projects/tmp/client.json");
#endif

    Server server;
    server.setStateFileName(stateFileName);
    server.setDataFileName(dataFileName);
    QElapsedTimer t;
    t.start();
    qDebug() << "Loading " << lammpsDumpFileName << "...";
    // server.loadXYZ(xyzFileName);
    server.loadLAMMPSBinary(lammpsDumpFileName);
    qDebug() << "Loading file took " << t.elapsed() << " ms.";
    while(true) {
        t.restart();
        server.update(clientStateFileName);
        qDebug() << "Updated position took " << t.restart() << " ms.";
        server.writePositions();
        server.writeState();
        qDebug() << "Writing file took " << t.restart() << " ms with " << server.particles().size() << " particles.";
        usleep(100000);
    }

    return 0;
}
