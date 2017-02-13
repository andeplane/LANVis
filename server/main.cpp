#include <QElapsedTimer>
#include <QDebug>
#include <iostream>
#include <unistd.h>
#include "server.h"
#include "xyzbinaryreader.h"

// #define KIF
using namespace std;

int main(int argc, char *argv[])
{
    Q_UNUSED(argc)
    Q_UNUSED(argv)
#ifdef KIF
    QString xyzFileName("/scratch/largesystem/dump.xyz");
    QString lammpsDumpFileName("/scratch/largesystem/dump.lmp.bin");
    QString dataFileName("/scratch/largesystem/dump.bin");
    QString stateFileName("/scratch/largesystem/state.json");
    QString clientStateFileName("/scratch/largesystem/client.json");
#else
    QString xyzFileName("/projects/tmp/dump.xyz");
    QString xyzBinaryFileName("/projects/LANVis/Deform_totxyz/Deform_totxyz/localdeform40.bin");
    QString lammpsDumpFileName("/projects/tmp/dump_20m.lmp.bin");
    // QString lammpsDumpFileName("/projects/tmp/dump_small.lmp.bin");
    // QString lammpsDumpFileName("/projects/tmp/dump_270m.lmp.bin");
    QString dataFileName("/projects/tmp/dump.bin");
    QString stateFileName("/projects/tmp/state.json");
    QString clientStateFileName("/projects/tmp/client.json");
    QString lockFileName("/projects/tmp/LANVis.lock");
#endif

//    Server server;
//    server.setStateFileName(stateFileName);
//    server.setDataFileName(dataFileName);
//    server.setLockFileName(lockFileName);
//    QElapsedTimer t;
//    t.start();
//    qDebug() << "Loading " << lammpsDumpFileName << "...";
//    // server.loadXYZ(xyzFileName);
//    // server.loadLAMMPSBinary(lammpsDumpFileName);
//    server.loadXYZBinary(xyzBinaryFileName);
//    qDebug() << "Loading file took " << t.elapsed() << " ms.";
//    while(true) {
//        t.restart();
//        bool anyChanges = server.update(clientStateFileName);
//        if(anyChanges) {
//            qDebug() << "Updated position on " << server.allParticles().size() << " particles took " << t.restart() << " ms.";
//            server.writePositions();
//            server.writeState();
//            qDebug() << "Writing file with " << server.particles().size() << " took " << t.restart() << " ms.";
//        }

//        usleep(300000);
//    }

    return 0;
}
