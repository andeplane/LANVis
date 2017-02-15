TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle

INCLUDEPATH += /usr/local/lib
QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS += -fopenmp
# DEFINES += _GLIBCXX_PARALLEL
SOURCES += \
    main.cpp \
    server.cpp \
    particlestyle.cpp \
    lammpsbinaryreader.cpp \
    chunk.cpp \
    xyzbinaryreader.cpp \
    import/lammpstextdumpimporter.cpp \
    state.cpp \
    generalparticle.cpp \
    ../client/clientstate.cpp \
    ../client/serversettings.cpp \
    import/xyzimporter.cpp \
    particlesubset.cpp \
    modifiers/modifier.cpp \
    import/xyzbinaryimporter.cpp \
    modifiers/selectionmodifier.cpp \
    modifiers/deleteselected.cpp

HEADERS += \
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
    particlesubset.h \
    modifiers/modifier.h \
    import/importers.h \
    import/xyzbinaryimporter.h \
    modifiers/selectionmodifier.h \
    modifiers/deleteselected.h \
    modifiers/modifiers.h
