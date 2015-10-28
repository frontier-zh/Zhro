#ifndef TASKTARGETDATABASE_H
#define TASKTARGETDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include "tasksourcedatabase.h"
#include <QSet>


class TaskTargetDatabase : public QObject
{
    Q_OBJECT
public:
    explicit TaskTargetDatabase(QObject *parent = 0);

    bool connectToDB(const QString& host, int port,
                     const QString& dbname,
                     const QString& taskTableName,
                     const QString& patternTableName,
                     const QString& uid,
                     const QString& pwd);
    bool closeDB();

    bool loadFromSourceDataBase(TaskSourceDatabase *sourcedb, const QString& ip);
    bool updateTaskScheculing();

signals:

public slots:
protected:
    bool processTask(TaskSourceDatabase *sourcedb,
                     TaskSourceDatabase::RESULT &tasks, int index, const QString& fromip);
    bool insertTask(TaskSourceDatabase::RESULT &tasks, int index, const QString& fromip);
    bool existsTask(QString &taskname);
    bool decidePattern(TaskSourceDatabase::RESULT &pattern);
    bool insertPattern(TaskSourceDatabase::RESULT &pattern);
    bool renamePattern(const QString& taskname, const QString& patternname);
    bool isPatternNameUsed(TaskSourceDatabase::RESULT &pattern, bool &error);

    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();

    void loadTaskFieldName();

private:
    QSqlDatabase _targetDB;

    QString _host, _dbname, _taskTableName, _patternTableName;
    quint16 _port;

    QSet<QString> _fields;
};

#endif // TASKTARGETDATABASE_H
