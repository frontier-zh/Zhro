#ifndef CALSHORTESTPATH_H
#define CALSHORTESTPATH_H

#include <QObject>
#include <QtCore>
#include <QDebug>
#include <QFile>

class CalShortestPath : public QObject
{
    Q_OBJECT
public:
    explicit CalShortestPath(QObject *parent = 0);

    void    obtainDoubleDot(QPointF,QPointF);
    void    obtainDoubleDot(QString,QString);
    void    shorestPaths(QList<QPointF>,QString);
    QString    pointFindCity(QPointF pt);
    QPointF    cityFindPoint(QString name);

signals:

public slots:
public:
    QString     shortPath;
private:
    QList<QList<QPointF> >  polyLines;
    QList<QPointF>          allPoints;
    QPointF     start;
    QPointF     finish;
    QMap<QString,QPointF>   mCitys;
    int         lenpath;

};

#endif // CALSHORTESTPATH_H
