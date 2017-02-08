#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QtQml>

#include "mysimulator.h"
#include "mousemover.h"
#include "clientstate.h"
#include "state.h"
#include "particles.h"

#include <vendor.h>

int main(int argc, char *argv[])
{
    // Fixes motion problems, see https://bugreports.qt.io/browse/QTBUG-53165
    qputenv("QSG_RENDER_LOOP", "basic");

    qmlRegisterType<MySimulator>("LANVis", 1, 0, "MySimulator");
    qmlRegisterType<State>("LANVis", 1, 0, "State");
    qmlRegisterType<ClientState>("LANVis", 1, 0, "ClientState");
    qmlRegisterType<Particles>("LANVis", 1, 0, "Particles");
    qmlRegisterType<MouseMover>("LANVis", 1, 0, "MouseMover");
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setMajorVersion(3);
    format.setMinorVersion(2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    // Application version
    QQmlApplicationEngine engine;
    qpm::init(app, engine);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
