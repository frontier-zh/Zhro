#include "tasksourcedatabase.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>


static const char* CONNECTION_NAME = "SOURCE CONNECTION";

TaskSourceDatabase::TaskSourceDatabase(QObject *parent) :
    QObject(parent)
{
}

bool
TaskSourceDatabase::connectToDB(const QString &host, int port,
                                const QString &dbname,
                                const QString &taskTableName,
                                const QString &patternTableName,
                                const QString &uid,
                                const QString &pwd,
                                const QString &condstring,
                                const QString &datasource)
{
    if (_sourceDB.isOpen()) {
        QLOG_TRACE() << "Already Opened";
        return false;
    }

    _host = host;
    _port = port;
    _dbname = dbname;
    _taskTableName = taskTableName;
    _patternTableName = patternTableName;
    _uid = uid;
    _pwd = pwd;
    _condstring = condstring;
    _datasouce = datasource;

    QStringList conn = QSqlDatabase::connectionNames();
    foreach( QString name, conn ){
        if( !name.compare(host) ){
            QSqlDatabase::removeDatabase(name);
            break;
        }
    }

    if (!QSqlDatabase::isDriverAvailable("QODBC")) {
        QLOG_TRACE() << "QODBC is not available";
        QLOG_TRACE() << "Available drivers:" << QSqlDatabase::drivers();
        return false;
    }

    _sourceDB = QSqlDatabase::addDatabase("QODBC", _host + _dbname + _taskTableName);
    _sourceDB.setDatabaseName(QString("DRIVER={SQL SERVER};"
                                      "DATABASE=%1;SERVER=%2;UID=%3;PWD=%4;")
                              .arg(_dbname)
                              .arg(_host)
                              .arg(_uid)
                              .arg(_pwd));

    if (!_sourceDB.open()) {
        QLOG_TRACE() << _sourceDB.lastError();
        return false;
    }

    return true;
}

bool
TaskSourceDatabase::closeDB()
{
    _sourceDB.close();
    _sourceDB = QSqlDatabase();
    QSqlDatabase::removeDatabase(CONNECTION_NAME);

    return true;
}

bool
TaskSourceDatabase::loadTasks(RESULT &result)
{
    result.fieldNames.clear();
    result.values.clear();

    QString sql = QString("SELECT * FROM %1 WHERE %2 ;").arg(_taskTableName).arg(_condstring);

    QSqlQuery query(sql, _sourceDB);
    QSqlRecord rec = query.record();
    for (int i = 0;i < rec.count();i++) {
        result.fieldNames.append(rec.fieldName(i));
    }

    while(query.next()) {
        QList<QVariant> list;

        for(int i = 0;i < rec.count();i++) {
            list.append(query.value(i));
        }

        result.values.append(list);
    }

    if (query.lastError().isValid()) {
        QLOG_TRACE() << query.lastError();
        return false;
    }

    return true;
}

bool
TaskSourceDatabase::loadPattern(const QString &patternname, RESULT &result)
{
    result.fieldNames.clear();
    result.values.clear();

    QString sql = QString("SELECT * FROM %1 WHERE PatternName=? ORDER BY FieldNumber, ID;").arg(_patternTableName);

    QSqlQuery query(_sourceDB);
    query.prepare(sql);
    query.addBindValue(patternname);
    if (!query.exec()) {
        QLOG_TRACE() << query.lastError();
        return false;
    }

    QSqlRecord rec = query.record();
    for (int i = 0;i < rec.count();i++) {
        result.fieldNames.append(rec.fieldName(i));
    }

    while(query.next()) {
        QList<QVariant> list;

        for(int i = 0;i < rec.count();i++) {
            list.append(query.value(i));
        }

        result.values.append(list);
    }

    if (query.lastError().isValid()) {
        QLOG_TRACE() << query.lastError();
        return false;
    }

    return true;
}

bool TaskSourceDatabase::loadDatagetOrigin(MODEL &model)
{
    model.clear();
    QString sql = QString("SELECT TOP 3000 * FROM %1 WHERE datasource = '%2' ORDER BY gettime DESC;").arg(_taskTableName).arg(_datasouce);

    QSqlQuery query(sql, _sourceDB);
    QSqlRecord rec = query.record();

    while(query.next()) {
        QMap<QString, QVariant> map;

        for(int i = 0;i < rec.count();i++) {
            map.insert(rec.fieldName(i).toLower(),query.value(i));
        }
        model.append(map);
    }

    if (query.lastError().isValid()) {
        QLOG_TRACE() << query.lastError();
        return false;
    }

    return true;
}
