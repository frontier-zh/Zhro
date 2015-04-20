#-------------------------------------------------
#
# Project created by QtCreator 2015-03-23T09:35:50
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UnifiedTools
TEMPLATE = app
DESTDIR = $$PWD/bin

LIBS += \
    -lpacket \
    -lwpcap \
    -lws2_32

SOURCES += main.cpp\
        mainwindow.cpp \
    dialog.cpp \
    urlencode.cpp \
    unicodetozh.cpp \
    timestamp.cpp \
    httpanalyse.cpp \
    capturethread.cpp \
    log.cpp

HEADERS  += mainwindow.h \
    dialog.h \
    urlencode.h \
    unicodetozh.h \
    timestamp.h \
    httpanalyse.h \
    pheader.h \
    capturethread.h \
    public_define.h \
    log.h

FORMS    += mainwindow.ui \
    dialog.ui \
    urlencode.ui \
    unicodetozh.ui \
    timestamp.ui \
    httpanalyse.ui

RESOURCES += \
    resource.qrc
RC_FILE    = UnifiedTools.rc
