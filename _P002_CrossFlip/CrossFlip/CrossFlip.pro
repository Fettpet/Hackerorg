#-------------------------------------------------
#
# Project created by QtCreator 2014-08-02T19:15:07
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = CrossFlip
CONFIG   += console
CONFIG   -= app_bundle
QMAKE_CXXFLAGS += -fopenmp -std=c++0x
QMAKE_CFLAGS += -fopenmp -std=c++0x
QMAKE_LFLAGS += -fopenmp
TEMPLATE = app


SOURCES += main.cpp \
    int_gf2.cpp \
    cross.cpp \
    crosstolinearsystem.cpp \
    pos.cpp \
    crosstobitset.cpp

HEADERS += \
    int_gf2.h \
    cross.h \
    crosstolinearsystem.h \
    pos.h \
    Eigen/Dense \
    Eigen/Sparse \
    crosstobitset.h
