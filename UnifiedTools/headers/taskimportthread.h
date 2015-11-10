#ifndef TASKIMPORTTHREAD_H
#define TASKIMPORTTHREAD_H

#include <QThread>
#include <QtCore>
#include "tasksourcedatabase.h"
#include "tasktargetdatabase.h"
#include "public_define.h"

class TaskImportThread : public QThread
{
    Q_OBJECT
public:
    explicit TaskImportThread(QObject *parent = 0);

    void    setHostList(QList<DBINFO> hosts){
            this->infos = hosts;
    }
protected:
    void    run();
signals:
    void    notifyConsole(DBINFO info);

public slots:

private:
    QList<DBINFO> infos;
};

#endif // TASKIMPORTTHREAD_H
