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

    while(true) {
        bool clientFileExists = server.update();
        if(clientFileExists) {
            usleep(25000);
        } else {
            qDebug() << "Client file does not exist. Waiting...";
            usleep(1000000);
        }
    }

    return 0;
}
