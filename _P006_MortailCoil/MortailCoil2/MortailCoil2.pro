TEMPLATE = app
CONFIG += console
CONFIG += app_bundle
CONFIG += qt
QT       += core gui network

QMAKE_CXXFLAGS += -std=c++11
SOURCES += main.cpp \
    World/world.cpp \
    Solver/dfs.cpp \
    World/region.cpp \
    Solver/regiondfs.cpp \
    Solver/Data/connection.cpp \
    Solver/Data/Move.cpp \
    Solver/Data/SavedMoveSequence.cpp \
    Solver/Data/pos.cpp

HEADERS += \
    World/world.h \
    Solver/dfs.h \
    World/region.h \
    Solver/regiondfs.h \
    Solver/Data/connection.h \
    Solver/Data/MoveDirection.h \
    Solver/Data/Move.h \
    Solver/Data/SavedMoveSequence.h \
    Solver/Data/pos.h

