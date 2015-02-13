#-------------------------------------------------
#
# Project created by QtCreator 2015-02-09T13:52:13
#
#-------------------------------------------------

QT       += core gui xlsx

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lnwt
TEMPLATE = app
DESTDIR = $$PWD/bin


SOURCES += main.cpp\
        mainwindow.cpp \
    datahandle.cpp \
    cvs_fileparse.cpp \
    res_fileparse.cpp

HEADERS  += mainwindow.h \
    datahandle.h \
    cvs_fileparse.h \
    res_fileparse.h

FORMS    += mainwindow.ui
RC_FILE   = zhro.rc
