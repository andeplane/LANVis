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
    xyzbinaryreader.cpp \
    import/particleimporter.cpp \
    import/lammpstextdumpimporter.cpp \
    state.cpp \
    generalparticle.cpp \
    ../client/clientstate.cpp \
    ../client/serversettings.cpp \
    import/xyzimporter.cpp \
    particlesubset.cpp

HEADERS += \
    xyzreader.h \
    ../client/particle.h \
    server.h \
    particlestyle.h \
    lammpsbinaryreader.h \
    chunk.h \
    xyzbinaryreader.h \
    import/particleimporter.h \
    import/lammpstextdumpimporter.h \
    state.h \
    generalparticle.h \
    ../client/clientstate.h \
    ../client/serversettings.h \
    import/xyzimporter.h \
    particlesubset.h
