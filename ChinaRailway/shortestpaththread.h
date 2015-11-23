#ifndef SHORTESTPATHTHREAD_H
#define SHORTESTPATHTHREAD_H

#include <QThread>
#include <QtCore>
#include <QDebug>

class ShortestPathThread : public QThread
{
    Q_OBJECT
public:
    explicit ShortestPathThread(QObject *parent = 0);
    void    setCityname(QString name1, QString name2){
        bname = name1;
        ename = name2;
    }

    QList<QPointF> getShortest() {
        return shortPath;
    }

signals:
public slots:

protected:
    void    run();
    void    obtainDoubleDot(QString,QString);
    void    shorestPaths(QList<QPointF>,QString);
    QString    pointFindCity(QPointF pt);
    QPointF    cityFindPoint(QString name);

private:

    QList<QList<QPointF> >  polyLines;
    QList<QPointF>          allPoints;
    QList<QPointF>          shortPath;
    QPointF     finish;
    QString     bname;
    QString     ename;
    QMap<QString,QPointF>   mCitys;
    int         lenpath;

};

#endif // SHORTESTPATHTHREAD_H
