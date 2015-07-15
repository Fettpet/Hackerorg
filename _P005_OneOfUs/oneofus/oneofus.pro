#-------------------------------------------------
#
# Project created by QtCreator 2014-06-27T21:15:40
#
#-------------------------------------------------
QMAKE_CXXFLAGS += -std=c++11
QT       += core

QT       -= gui

TARGET = oneofus
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    field.cpp \
    pos.cpp \
    fieldtocnf.cpp \
    exactlyone.cpp

HEADERS += \
    fieldd.h \
    pos.h \
    fieldtocnf.h \
    exactlyone.h
