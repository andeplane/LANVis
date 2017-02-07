#include <QElapsedTimer>
#include <QDebug>
#include <iostream>
#include <unistd.h>
#include "server.h"

using namespace std;

int main(int argc, char *argv[])
{
    QString xyzFileName("/projects/tmp/dump.xyz");
    QString outFileName("/projects/tmp/dump.bin");
    QString clientStateFileName("/projects/tmp/client.json");

    Server server;
    QElapsedTimer t;
    t.start();
    server.loadXYZ(xyzFileName);
    qDebug() << "Loading " << xyzFileName << " took " << t.elapsed() << " ms.";
    while(true) {
        t.restart();
        server.update(clientStateFileName);
        qDebug() << "Updated position took " << t.restart() << " ms.";
        server.writePositions(outFileName);
        qDebug() << "Writing file took " << t.restart() << " ms.";
        usleep(100000);
    }

    return 0;
}
