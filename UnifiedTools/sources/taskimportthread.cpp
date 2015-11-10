#include "taskimportthread.h"

TaskImportThread::TaskImportThread(QObject *parent) :
    QThread(parent)
{
}

void TaskImportThread::run()
{
    TaskTargetDatabase * target = new TaskTargetDatabase();
    if (!target->connectToDB(infos[0].host,
                             infos[0].port,
                             infos[0].dbname,
                             infos[0].tasktablename,
                             infos[0].patterntablename,
                             infos[0].uid,
                             infos[0].pwd))
    {
        QLOG_TRACE() << "Failed to connect TARGET Database" << infos[0].host
                 << infos[0].dbname;
        emit    notifyConsole(infos[0]);
        return;
    }

    for(int i = 1; i < infos.size();i++) {
        TaskSourceDatabase source;
        QLOG_TRACE() << "Connecting to " << infos[i].host;
        QLOG_TRACE() << QString("%1/%2").arg(i).arg(infos.size());

        if (!source.connectToDB(infos[i].host,
                                infos[i].port,
                                infos[i].dbname,
                                infos[i].tasktablename,
                                infos[i].patterntablename,
                                infos[i].uid,
                                infos[i].pwd,
                                infos[i].condstring,
                                infos[i].datasource))
        {
            QLOG_TRACE() << "Failed to connect SOURCE Database " << infos[i].host
                        << infos[i].dbname;
            emit    notifyConsole(infos[i]);
            target->closeDB();
            return;
        }
        if (!target->loadFromSourceDataBase(&source, infos[i].host)) {
            source.closeDB();
            target->closeDB();
            return;
        } else {
            source.closeDB();
        }
    }
    if( !target->updateTaskScheculing() ){
        QLOG_TRACE() << "Finally Failed to Update TaskScheculing Table.";
        target->closeDB();
        return;
    }
    target->closeDB();

}
