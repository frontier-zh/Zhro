#-------------------------------------------------
#
# Project created by QtCreator 2014-11-05T08:40:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Graphics
TEMPLATE = app
DESTDIR = $$PWD/bin


SOURCES += main.cpp\
        mainwidget.cpp \
    basepluginwidget.cpp \
    basemapwidget.cpp \
    mapviewstate.cpp \
    flowdatastate.cpp \
    lptextitem.cpp \
    lppolylineitem.cpp \
    lprounditem.cpp \
    calshortestpath.cpp \
    shortestpaththread.cpp

HEADERS  += mainwidget.h \
    basepluginwidget.h \
    basemapwidget.h \
    mapviewstate.h \
    flowdatastate.h \
    lptextitem.h \
    lppolylineitem.h \
    public_define.h \
    lprounditem.h \
    calshortestpath.h \
    shortestpaththread.h
