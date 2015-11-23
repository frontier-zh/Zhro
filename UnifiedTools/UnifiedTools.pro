#-------------------------------------------------
#
# Project created by QtCreator 2015-03-23T09:35:50
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UnifiedTools
TEMPLATE = app
DESTDIR = $$PWD/bin

LIBS += \
    -lpacket \
    -lwpcap \
    -lws2_32
INCLUDEPATH += headers/

SOURCES += sources/main.cpp\
        sources/mainwindow.cpp \
        sources/dialog.cpp \
        sources/urlencode.cpp \
        sources/unicodetozh.cpp \
        sources/timestamp.cpp \
        sources/httpanalyse.cpp \
        sources/capturethread.cpp \
        sources/log.cpp \
        sources/taskimport.cpp \
        sources/tasksourcedatabase.cpp \
        sources/tasktargetdatabase.cpp \
        sources/datahandle.cpp \
        sources/datacomparison.cpp \
        sources/taskimportthread.cpp \
        sources/datacomparisonthread.cpp \
        sources/distancelatlng.cpp \
        sources/drawpolygon.cpp \
        sources/refinelatlng.cpp \
        sources/tcpclisocket.cpp

HEADERS  += headers/mainwindow.h \
        headers/dialog.h \
        headers/urlencode.h \
        headers/unicodetozh.h \
        headers/timestamp.h \
        headers/httpanalyse.h \
        headers/pheader.h \
        headers/capturethread.h \
        headers/public_define.h \
        headers/log.h \
        headers/taskimport.h \
        headers/tasksourcedatabase.h \
        headers/tasktargetdatabase.h \
        headers/datahandle.h \
        headers/datacomparison.h \
        headers/taskimportthread.h \
        headers/datacomparisonthread.h \
        headers/distancelatlng.h \
        headers/public_helper.h \
        headers/drawpolygon.h \
        headers/refinelatlng.h \
        headers/tcpclisocket.h


FORMS    += forms/mainwindow.ui \
        forms/dialog.ui \
        forms/urlencode.ui \
        forms/unicodetozh.ui \
        forms/timestamp.ui \
        forms/httpanalyse.ui \
        forms/taskimport.ui \
        forms/datacomparison.ui \
        forms/distancelatlng.ui \
        forms/drawpolygon.ui \
        forms/refinelatlng.ui \
        forms/tcpclisocket.ui


RESOURCES += \
        resource.qrc

RC_FILE    = UnifiedTools.rc
