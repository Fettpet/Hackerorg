#-------------------------------------------------
#
# Project created by QtCreator 2014-08-13T22:15:34
#
#-------------------------------------------------

QT       += core
QMAKE_CXXFLAGS+= -fopenmp
QMAKE_LFLAGS +=  -fopenmp
QT       -= gui
QMAKE_CXXFLAGS += -std=c++11

TARGET = modulo
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    Solver/solver.cpp \
    Modulo/field.cpp \
    Modulo/game.cpp \
    Modulo/pos.cpp \
    Modulo/tile.cpp \
    IO/IO.cpp \
    Exceptions/modiszero.cpp \
    Exceptions/stringexception.cpp \
    Solver/deepsearch.cpp \
    Solver/Sat/mod2tosat.cpp \
    Solver/Sat/satbaseclass.cpp \
    Solver/Sat/mod3tosat.cpp \
    Solver/Sat/mod4tosat.cpp \
    Solver/Sat/tosathalf.cpp \
    Solver/Sat/tosat.cpp \
    Solver/Planning/pddladl.cpp \
    Solver/Planning/pddlbaseclass.cpp \
    Solver/Planning/pddlstrips.cpp \
    Complextest/statistic.cpp \
    Complextest/compositiontile.cpp \
    Solver/Planning/pddladlconst.cpp \
    Solver/Planning/pddlstripsconst.cpp \
    Solver/Planning/pddladlnumerics.cpp \
    Solver/tabusearch.cpp

HEADERS += \
    Solver/solver.h \
    Modulo/field.h \
    Modulo/game.h \
    Modulo/pos.h \
    Modulo/tile.h \
    IO/IO.h \
    Defines/compileoptions.h \
    Exceptions/modiszero.h \
    Exceptions/stringexception.h \
    Solver/deepsearch.h \
    Solver/Sat/mod2tosat.h \
    Solver/Sat/satbaseclass.h \
    Solver/Sat/mod3tosat.h \
    Solver/Sat/mod4tosat.h \
    Solver/Sat/tosathalf.h \
    Solver/Sat/tosat.h \
    Solver/Planning/pddladl.h \
    Solver/Planning/pddlbaseclass.h \
    Solver/Planning/pddlstrips.h \
    Complextest/statistic.h \
    Complextest/compositiontile.h \
    Solver/Planning/pddladlconst.h \
    Solver/Planning/pddlstripsconst.h \
    Solver/Planning/pddladlnumerics.h \
    Solver/tabusearch.h
