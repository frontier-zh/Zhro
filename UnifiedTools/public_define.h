#ifndef PUBLIC_DEFINE_H
#define PUBLIC_DEFINE_H

#include <QtDebug>
#include "log.h"
#include <QMessageBox>

typedef QList< QMap<QString,QVariant> >   MODEL;
struct DBINFO {
    QString host;
    int port;
    QString dbname, tasktablename, patterntablename, condstring;
    QString uid, pwd;
    QString datasource;
};

#ifndef QT_NO_DEBUG
#define DBG_FUNCTION QLOG_TRACE() << __FUNCTION__;
#else
#define DBG_FUNCTION
#endif

#endif // PUBLIC_DEFINE_H
