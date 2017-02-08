TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

SOURCES += \
    main.cpp \
    xyzreader.cpp \
    server.cpp \
    atomstyle.cpp \
    lammpsbinaryreader.cpp \
    chunk.cpp

HEADERS += \
    xyzreader.h \
    ../client/particle.h \
    server.h \
    atomstyle.h \
    lammpsbinaryreader.h \
    chunk.h
