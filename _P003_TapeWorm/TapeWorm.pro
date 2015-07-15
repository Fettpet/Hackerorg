TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += qt
QMAKE_CXXFLAGS += -std=c++11 -fopenmp


QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2

QMAKE_CXXFLAGS_RELEASE *= -O3
QMAKE_CXXFLAGS += -march=native -fopenmp
QMAKE_LFLAGS -= -O -O1 -O2
QMAKE_LFLAGS += -O3
SOURCES += main.cpp \
    World/pos.cpp \
    World/world.cpp \
    World/worm.cpp \
    World/wormmap.cpp \
    Heuristics/astarheuristics.cpp \
    Heuristics/easywormheuristics.cpp \
    Heuristics/getonefieldfree.cpp \
    Heuristics/getwormtofield.cpp \
    Calculation/astarboost.cpp \
    Calculation/astarcalculation.cpp \
    Calculation/astargraph.cpp \
    Calculation/astarvisitor.cpp \
    Exceptions/nowayfound.cpp \
    Praprozessor/easywormstohome.cpp \
    Praprozessor/recolor.cpp \
    Tools/worldparts.cpp \
    World/worlddeep.cpp \
    Calculation/depthfirst.cpp \
    Exceptions/depthwayfound.cpp \
    Exceptions/onewormathome.cpp \
    Calculation/PDDL/pddlsolution.cpp \
    Calculation/dfs_astar.cpp \
    Exceptions/depthrestart.cpp

HEADERS += \
    World/pos.h \
    World/world.h \
    World/worm.h \
    World/wormmap.h \
    Heuristics/astarheuristics.h \
    Heuristics/easywormheuristics.h \
    Heuristics/getonefieldfree.h \
    Heuristics/getwormtofield.h \
    Calculation/astarboost.h \
    Calculation/astarcalculation.h \
    Calculation/astargraph.h \
    Calculation/astarvisitor.h \
    Exceptions/nowayfound.h \
    Praprozessor/easywormstohome.h \
    Praprozessor/recolor.h \
    Tools/worldparts.h \
    World/worlddeep.h \
    Calculation/depthfirst.h \
    Exceptions/depthwayfound.h \
    Exceptions/onewormathome.h \
    Calculation/PDDL/pddlsolution.h \
    Calculation/dfs_astar.h \
    Exceptions/depthrestart.h

