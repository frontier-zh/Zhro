#ifndef DATACOMPARISONTHREAD_H
#define DATACOMPARISONTHREAD_H

#include <QThread>
#include "public_define.h"
#include "tasksourcedatabase.h"

class DataComparisonThread : public QThread
{
    Q_OBJECT
public:
    explicit DataComparisonThread(QObject *parent = 0);

    void    setHostList(QList<DBINFO> hosts){
            this->infos = hosts;
    }
    QList<QStringList> getResult(){
        return  result;
    }

protected:
    void    run();
signals:
    void    stopFeekback(QString infomation);

public slots:

private:
    QList<DBINFO> infos;
    bool          match;
    long          total;
    QList<QStringList>  result;
};

#endif // DATACOMPARISONTHREAD_H
