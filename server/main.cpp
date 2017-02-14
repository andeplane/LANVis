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

    Server server;
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
