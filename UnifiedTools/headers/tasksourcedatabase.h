#ifndef TASKSOURCEDATABASE_H
#define TASKSOURCEDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QList>
#include <QMap>
#include "public_define.h"

class TaskSourceDatabase : public QObject
{
    Q_OBJECT
public:
    explicit TaskSourceDatabase(QObject *parent = 0);

    struct RESULT {
        QList<QVariant> fieldNames;
        QList<QList<QVariant> > values;
    };

    bool connectToDB(const QString& host, int port,
                     const QString& dbname,
                     const QString& taskTableName,
                     const QString& patternTableName,
                     const QString& uid,
                     const QString& pwd,
                     const QString &condstring, const QString &datasource);
    bool closeDB();

    bool loadTasks(RESULT &result);
    bool loadPattern(const QString& patternname, RESULT& result);
    bool loadDatagetOrigin(MODEL& model);

signals:

public slots:
private:
    QSqlDatabase _sourceDB;

    QString _host, _dbname, _taskTableName, _patternTableName, _condstring, _uid, _pwd, _datasouce;
    quint16 _port;
};

#endif // TASKSOURCEDATABASE_H
