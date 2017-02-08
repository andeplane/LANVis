TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

SOURCES += \
    main.cpp \
    xyzreader.cpp \
    server.cpp \
    particlestyle.cpp \
    lammpsbinaryreader.cpp \
    chunk.cpp \
    xyzbinaryreader.cpp

HEADERS += \
    xyzreader.h \
    ../client/particle.h \
    server.h \
    particlestyle.h \
    lammpsbinaryreader.h \
    chunk.h \
    xyzbinaryreader.h
