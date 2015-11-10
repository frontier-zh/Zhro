#include "tasktargetdatabase.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include "tasksourcedatabase.h"
#include "public_define.h"

TaskTargetDatabase::TaskTargetDatabase(QObject *parent) :
    QObject(parent), _port(3306)
{
}

bool
TaskTargetDatabase::connectToDB(const QString &host, int port,
                                const QString &dbname,
                                const QString &taskTableName,
                                const QString &patternTableName,
                                const QString &uid, const QString& pwd)
{
    _host = host;
    _port = port;
    _dbname = dbname;
    _taskTableName = taskTableName;
    _patternTableName = patternTableName;

    if (!QSqlDatabase::isDriverAvailable("QMYSQL")) {
        QLOG_TRACE() << "QMYSQL is not available";
        QLOG_TRACE() << "Available drivers:" << QSqlDatabase::drivers();
        return false;
    }

    _targetDB = QSqlDatabase::addDatabase("QMYSQL", _host + QString::number(_port));
    _targetDB.setHostName(_host);
    if (_port > 0) {
        _targetDB.setPort(_port);
    }
    _targetDB.setDatabaseName(_dbname);
    _targetDB.setUserName(uid);
    _targetDB.setPassword(pwd);

    if (!_targetDB.open()) {
        QLOG_TRACE() << _targetDB.lastError();
        return false;
    }

    loadTaskFieldName();

    return true;
}

void
TaskTargetDatabase::loadTaskFieldName()
{
    QString sql = "SELECT * FROM `task` LIMIT 1";
    QSqlQuery query(sql, _targetDB);
    QSqlRecord rec = query.record();

    for(int i = 0;i < rec.count();i++) {
        _fields.insert(rec.fieldName(i));
    }
}

bool
TaskTargetDatabase::insertTask(TaskSourceDatabase::RESULT &tasks, int index, const QString& fromip)
{
    QString sql = QString("INSERT INTO `%1`(").arg(_taskTableName);
    QList<int> validFields;
    QString taskname;

    for (int j = 1;j < tasks.fieldNames.size();j++) {
        if (_fields.contains(tasks.fieldNames.at(j).toString())) {
            validFields.append(j);
            if (tasks.fieldNames.at(j).toString().compare("TaskName", Qt::CaseInsensitive) == 0) {
                taskname = tasks.values[index][j].toString();
            }
        } else {
            QLOG_TRACE() << "Unknown Fields" << tasks.fieldNames.at(j);
        }
    }

    for (int j = 0;j < validFields.size();j++) {
        sql += QString("`%1`").arg(tasks.fieldNames.at(validFields[j]).toString());
        if (j < validFields.size() - 1)  {
            sql += ",";
        }
    }
    sql += ") VALUES(";
    for (int j = 0;j < validFields.size();j++) {
        sql += "?";
        if (j < validFields.size() - 1)  {
            sql += ",";
        }
    }
    sql += ")";

    QSqlQuery query(_targetDB);
    query.prepare(sql);
    for (int j = 0;j < validFields.size();j++) {
        query.addBindValue(tasks.values[index][validFields[j]]);
    }

    if (!query.exec()) {
        QLOG_TRACE() << "FAILED TO ADD TASK";
        QLOG_TRACE() << query.lastError();
        QLOG_TRACE() << query.executedQuery();
        return false;
    }

    Q_ASSERT(!taskname.isEmpty());
    Q_ASSERT(!fromip.isEmpty());

    QString sql2 = "INSERT INTO task_import(task_name, from_ip, import_time) VALUES(?,?,NOW()"
            ") ON DUPLICATE KEY UPDATE from_ip=VALUES(from_ip),import_time=NOW()";
    QSqlQuery query2(_targetDB);
    query2.prepare(sql2);
    query2.addBindValue(taskname);
    query2.addBindValue(fromip);

    if (!query2.exec()) {
        QLOG_TRACE() << "FAILED TO ADD TASK IMPORT";
        QLOG_TRACE() << query2.lastError();
        QLOG_TRACE() << query2.executedQuery();
        return false;
    }

    return true;
}

bool
TaskTargetDatabase::existsTask(QString &taskname)
{
    if( taskname.isEmpty() ){
        return false;
    }
    QString sql = QString("SELECT COUNT(*) FROM %1 WHERE taskname ='%2' ").arg(_taskTableName).arg(taskname);


    QSqlQuery   que(_targetDB);
    if( !que.exec(sql) ){
        return false;
    }
    if( que.next() ){
        if( que.value(0).toInt() > 0 ){
            return true;
        }
    }
    return false;
}

bool
TaskTargetDatabase::processTask(TaskSourceDatabase *sourcedb,
                                TaskSourceDatabase::RESULT &tasks, int index, const QString& fromip)
{
    const QList<QVariant>& task = tasks.values.at(index);
    QString taskname = task[1].toString();
    QString tablename = task[11].toString();
    QString patternname = task[2].toString();


    if (existsTask(taskname)) {
        QLOG_TRACE() << "The same name task abandon insert! " << taskname;
        return true;
    }
    if (!insertTask(tasks, index, fromip)) {
        return false;
    }

    TaskSourceDatabase::RESULT pattern;
    if (!sourcedb->loadPattern(patternname, pattern)) {
        QLOG_TRACE() << "Failed to load patterns:" << patternname;
        return false;
    }

    bool needrenamepattern = false;
    QString citycode = taskname.split("_").at(0);
    if (tablename.contains("dataget_") &&
            patternname.startsWith("all") && citycode != "all") {
        QString newname = QString("%1_%2").arg(citycode)
            .arg(patternname);
        TaskSourceDatabase::RESULT newpattern = pattern;
        for(int i = 0;i < newpattern.values.size();i++) {
            newpattern.values[i][1] = newname;
        }
        patternname = newname;
        pattern = newpattern;
        needrenamepattern = true;
    }
    //Remove the same pattern
    if( decidePattern(pattern))
    {
        bool error = false;
        bool used = isPatternNameUsed(pattern, error);
        if (error) {
            return false;
        }
        if (used) {
            needrenamepattern = true;
            int start = 1;
            while(true) {
                QString newname = QString("%1_%2").arg(patternname).arg(start);
                TaskSourceDatabase::RESULT newpattern = pattern;
                for(int i = 0;i < newpattern.values.size();i++) {
                    newpattern.values[i][1] = newname;
                }

                bool isused = isPatternNameUsed(newpattern, error);
                if (error) {
                    return false;
                }
                if (!isused) {
                    // QLOG_TRACE() << patternname << "RENAMED TO" << newname;
                    patternname = newname;
                    pattern = newpattern;
                    break;
                }
                start++;
            }
        }
        if(!insertPattern(pattern)){
            return  false;
        }
        if (needrenamepattern) {
            if (!renamePattern(taskname, patternname)) {
                return false;
            }
        }
    }
    else
    {
        if (!renamePattern(taskname, patternname)) {
            return false;
        }
    }
    return true;
}

bool
TaskTargetDatabase::closeDB()
{
    _targetDB.close();
    return true;
}

bool
TaskTargetDatabase::loadFromSourceDataBase(TaskSourceDatabase *sourcedb, const QString& ip)
{
    TaskSourceDatabase::RESULT tasks;
    if (!sourcedb->loadTasks(tasks)) {
        return false;
    }
    QLOG_TRACE() <<  tasks.values.size() << "tasks found";

    for(int i = 0;i < tasks.values.size();i++) {
        if (!beginTransaction()) {
            return false;
        }
        if (!processTask(sourcedb, tasks, i, ip)) {
            rollbackTransaction();
            return false;
        } else {
            if (!commitTransaction()) {
                return false;
            }
        }
    }

    return true;
}

bool
TaskTargetDatabase::updateTaskScheculing()
{

    QString sql = QString("SELECT ID FROM %1 WHERE ID NOT IN(SELECT task_id FROM task_scheculing) ;").arg(_taskTableName);

    QSqlQuery   que(_targetDB);
    if(!que.exec(sql)){
        QLOG_TRACE() << "Obtain Failed Non Exist ID of TaskScheculing" << que.lastError();
        return false;
    }
    QSqlQuery   query(_targetDB);
    while(que.next()){
        QString sql1 = QString("INSERT INTO task_scheculing (task_id,minute,hour,day,month,week,valid,last_run) "
                              "values(%1,'*','*','*','*','*',0,now());").arg(que.value(0).toInt());
        if(!query.exec(sql1)){
            QLOG_TRACE() << "Update Failed to Insert Into TaskScheculing" << query.lastError();
            return false;
        }
    }

    if (que.lastError().isValid()) {
        QLOG_TRACE() << query.lastError();
        return false;
    }

    return true;
}

bool
TaskTargetDatabase::beginTransaction()
{
    QSqlQuery query("START TRANSACTION;", _targetDB);
    if (!query.exec()) {
        QLOG_TRACE() <<  query.lastError();
        QLOG_TRACE() <<  query.executedQuery();
        return false;
    }
    return true;
}

bool
TaskTargetDatabase::commitTransaction()
{
    QSqlQuery query("COMMIT;", _targetDB);
    if (!query.exec()) {
        QLOG_TRACE() <<  query.lastError();
        QLOG_TRACE() <<  query.executedQuery();
        return false;
    }
    return true;
}

bool
TaskTargetDatabase::rollbackTransaction()
{
    QSqlQuery query("ROLLBACK;", _targetDB);
    if (!query.exec()) {
        QLOG_TRACE() <<  query.lastError();
        QLOG_TRACE() <<  query.executedQuery();
        return false;
    }
    return true;
}

bool
TaskTargetDatabase::isPatternNameUsed(TaskSourceDatabase::RESULT &pattern,
                                      bool &error)
{
    error = false;

    for(int i = 0;i < pattern.values.size();i++) {
        QString pname = pattern.values[i][1].toString();

        QString sql = QString("SELECT * FROM `%1` WHERE"
                              " `PatternName`=?")
                .arg(_patternTableName);
        QSqlQuery query(_targetDB);
        query.prepare(sql);
        query.addBindValue(pname);
        if (!query.exec()) {
            error = true;
            return false;
        }
        if (query.next()) {
            return true;
        }
    }

    return false;
}

bool
TaskTargetDatabase::decidePattern(TaskSourceDatabase::RESULT &pattern)
{
    bool repeatflg = false;
    for(int i = 0;i < pattern.values.size();i++) {
        QString sql = QString("SELECT COUNT(*) FROM `%1` WHERE ").arg(_patternTableName);
        for (int j = 1;j < pattern.fieldNames.size();j++) {
            if( !pattern.values[i][j].isNull()){
                sql += QString("`%1`").arg(pattern.fieldNames[j].toString().toLower());
                sql += " = ? AND ";
            }
        }
        sql = sql.mid(0,sql.length()-4);
        QSqlQuery query(_targetDB);
        query.prepare(sql);
        for (int j = 1;j < pattern.fieldNames.size();j++) {
            if( !pattern.values[i][j].isNull()){
                query.addBindValue(pattern.values[i][j]);
            }
        }
        if (!query.exec()) {
            QLOG_TRACE() << query.lastError();
            QLOG_TRACE() << query.executedQuery();
            return false;
        }else if( query.next()){
            if( query.value(0).toInt() == 0){
                repeatflg = true;
            }
        }
    }
    return  repeatflg;
}
bool
TaskTargetDatabase::insertPattern(TaskSourceDatabase::RESULT &pattern)
{
    for(int i = 0;i < pattern.values.size();i++) {
        QString sql = QString("INSERT INTO `%1`(").arg(_patternTableName);
        for (int j = 1;j < pattern.fieldNames.size();j++) {
            sql += QString("`%1`").arg(pattern.fieldNames[j].toString().toLower());
            if (j < pattern.fieldNames.size() - 1) {
                sql += ",";
            }
        }
        sql += ") VALUES(";
        for (int j = 1;j < pattern.fieldNames.size();j++) {
            sql += "?";
            if (j < pattern.fieldNames.size() - 1) {
                sql += ",";
            }
        }
        sql += ");";

        QSqlQuery query(_targetDB);
        query.prepare(sql);
        for (int j = 1;j < pattern.fieldNames.size();j++) {
            query.addBindValue(pattern.values[i][j]);
        }

        if (!query.exec()) {
            QLOG_TRACE() << query.lastError();
            QLOG_TRACE() << query.executedQuery();
            return false;
        }
    }

    return true;
}

bool
TaskTargetDatabase::renamePattern(const QString &taskname,
                                  const QString &patternname)
{
    QString sql = QString("UPDATE `%1` SET `patternname`=? WHERE `taskname`=?")
            .arg(_taskTableName);
    QSqlQuery query(_targetDB);
    query.prepare(sql);
    query.addBindValue(patternname);
    query.addBindValue(taskname);

    if (!query.exec()) {
        QLOG_TRACE() << query.lastError();
        QLOG_TRACE() << query.executedQuery();
        return false;
    }
    Q_ASSERT(query.numRowsAffected() >= 0);

    return true;
}
