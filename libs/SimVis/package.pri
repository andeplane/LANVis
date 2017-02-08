include(libs/ShaderNodes/package.pri)

QT += widgets opengl openglextensions qml quick 3dcore 3drender 3dinput 3dquick

INCLUDEPATH += $$PWD/src
QML_IMPORT_PATH += $$PWD/src/imports/

RESOURCES += \
    $$PWD/src/simvis_shaders.qrc \
    $$PWD/src/imports/simvis_imports.qrc

SOURCES += \
    $$PWD/src/core/simulator.cpp \
    $$PWD/src/core/simvisplugin.cpp \
    $$PWD/src/render/geometry/pointgeometry.cpp \
    $$PWD/src/render/geometry/spherespointgeometry.cpp \
    $$PWD/src/render/geometry/spheredata.cpp \
    $$PWD/src/render/geometry/cylinderdata.cpp \
    $$PWD/src/render/geometry/uniformgridgeometry.cpp \
    $$PWD/src/utils/marchingcubestabletexture.cpp \
    $$PWD/src/render/geometry/bonddata.cpp

HEADERS += \
    $$PWD/src/core/simulator.h \
    $$PWD/src/core/simvisplugin.h \
    $$PWD/src/SimVis/Simulator \
    $$PWD/src/render/geometry/pointgeometry.h \
    $$PWD/src/render/geometry/spherespointgeometry.h \
    $$PWD/src/render/geometry/spheredata.h \
    $$PWD/src/SimVis/SphereData \
    $$PWD/src/render/geometry/cylinderdata.h \
    $$PWD/src/render/geometry/uniformgridgeometry.h \
    $$PWD/src/SimVis/CylinderData \
    $$PWD/src/utils/marchingcubestabletexture.h \
    $$PWD/src/render/geometry/bonddata.h \
    $$PWD/src/SimVis/BondData
