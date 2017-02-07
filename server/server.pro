TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

SOURCES += \
    main.cpp \
    ../client/xyzreader.cpp \
    server.cpp \
    atomstyle.cpp

HEADERS += \
    particle.h \
    ../client/xyzreader.h \
    server.h \
    atomstyle.h
