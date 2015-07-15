#-------------------------------------------------
#
# Project created by QtCreator 2013-03-11T13:24:45
#
#-------------------------------------------------

QT       += core gui network

TARGET = RunAwayRobot
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    downloadmanager.cpp \
    robotsworld.cpp \
    robotstree.cpp \
    point.cpp \
    bestpathentry.cpp \
    robotsbacktracking.cpp \
    bigInt/BigUnsignedInABase.cc \
    bigInt/BigUnsigned.cc \
    bigInt/BigIntegerUtils.cc \
    bigInt/BigIntegerAlgorithms.cc \
    bigInt/BigInteger.cc

HEADERS  += mainwindow.h \
    downloadmanager.h \
    robotsworld.h \
    robotstree.h \
    point.h \
    bestpathentry.h \
    robotsbacktracking.h \
    bigInt/NumberlikeArray.hh \
    bigInt/BigUnsignedInABase.hh \
    bigInt/BigUnsigned.hh \
    bigInt/BigIntegerUtils.hh \
    bigInt/BigIntegerLibrary.hh \
    bigInt/BigIntegerAlgorithms.hh \
    bigInt/BigInteger.hh

FORMS    += mainwindow.ui
