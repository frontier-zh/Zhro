#include "calshortestpath.h"
#include <QtMath>

CalShortestPath::CalShortestPath(QObject *parent) :
    QObject(parent), shortPath(""), lenpath(0)
{
    QFile f1("railway.txt");
    if( !f1.exists() ){
        qDebug() << "file railway.txt isn't exist";
        return;
    }
    if( !f1.open(QIODevice::ReadOnly|QIODevice::Text) ){
        qDebug() << "file railway.txt open failed";
        return;
    }
    QTextStream in(&f1);
    QString  line= "";
    polyLines.clear();
    mCitys.clear();
    allPoints.clear();
    while( !(line = in.readLine()).isEmpty() ){
        QList<QPointF> poly;
        QString  sline = line.split("|").at(1);
        foreach(QString spoint, sline.split(";")){
            QString city = spoint.split(",").at(0);
            QPointF pt(spoint.split(",").at(1).toFloat(),spoint.split(",").at(2).toFloat());
            poly.append(pt);
            allPoints.append(pt);
            mCitys.insert(city,pt);
        }
        if( poly.size() ){
            polyLines.append(poly);
        }
    }
    f1.close();
    qDebug() << "total line num is:" << polyLines.size();
    qDebug() << "total dots num is:" << allPoints.size() << "|" << mCitys.size();

}

void
CalShortestPath::obtainDoubleDot(QPointF begin, QPointF end)
{
    qDebug() << __FUNCTION__;
    if( !allPoints.contains(begin) || !allPoints.contains(end) ){
        qDebug() << "another one point isn't contain in allPoints";
        return;
    }
    start = begin;
    finish = end;

    QList<QPointF> lastaround;
    lastaround.append(begin);
    shorestPaths(lastaround,pointFindCity(begin));
    qDebug() << "task execute finished";
}

void
CalShortestPath::obtainDoubleDot(QString name1, QString name2)
{
    qDebug() << __FUNCTION__;
    QPointF begin = cityFindPoint(name1);
    QPointF end = cityFindPoint(name2);
    if( !allPoints.contains(begin) || !allPoints.contains(end) ){
        qDebug() << "another one point isn't contain in allPoints";
        return;
    }
    start = begin;
    finish = end;

    QList<QPointF> lastaround;
    lastaround.append(begin);
    shorestPaths(lastaround,pointFindCity(begin));
    qDebug() << "task execute finished";
}

void
CalShortestPath::shorestPaths(QList<QPointF> lastaround, QString path)
{
    //qDebug() << "[" << path << "]";
    QString  currPath = path;
    int i = 0;
    if( lenpath && path.size() > lenpath ){
        return;
    }

    QPointF pt = lastaround.at(lastaround.size()-1);
    for( ; i < polyLines.size(); i++ ){

        QList<QPointF> item = polyLines.at(i);
        int k = item.indexOf(pt);
        if( k != -1 ){
            if( k == 0 ){
                if( lastaround.contains(item.at(k+1)) ){
                    continue;
                }
                QList<QPointF> around = lastaround;
                around.append(item.at(k+1));
                QString adhere = pointFindCity(item.at(k+1));
                path = currPath;
                path.append(adhere);
                if(item.at(k+1) == finish ){
                    qDebug() << path;
                    if( !lenpath ){
                        shortPath = path;
                        lenpath = path.size();
                    }else if( path.size() <= lenpath ){
                        shortPath = path;
                        lenpath = path.size();
                    }
                    return;
                }
                shorestPaths(around,path);
            }else if( k == item.size() - 1 ){
                if( lastaround.contains(item.at(k-1)) ){
                    continue;
                }
                QList<QPointF> around = lastaround;
                around.append(item.at(k-1));
                QString adhere = pointFindCity(item.at(k-1));
                path = currPath;
                path.append(adhere);
                if(item.at(k-1) == finish ){
                    qDebug() << path;
                    if( !lenpath ){
                        shortPath = path;
                        lenpath = path.size();
                    }else if( path.size() <= lenpath ){
                        shortPath = path;
                        lenpath = path.size();
                    }
                    return;
                }
                shorestPaths(around,path);
            }else{
                if( !lastaround.contains(item.at(k-1))){
                    QList<QPointF> around = lastaround;
                    around.append(item.at(k-1));
                    QString adhere = pointFindCity(item.at(k-1));
                    path = currPath;
                    path.append(adhere);
                    if(item.at(k-1) == finish ){
                        qDebug() << path;
                        if( !lenpath ){
                            shortPath = path;
                            lenpath = path.size();
                        }else if( path.size() <= lenpath ){
                            shortPath = path;
                            lenpath = path.size();
                        }
                        return;
                    }
                    shorestPaths(around,path);
                }
                if( !lastaround.contains(item.at(k+1)) ){
                    QList<QPointF> around = lastaround;
                    around.append(item.at(k+1));
                    QString adhere = pointFindCity(item.at(k+1));
                    path = currPath;
                    path.append(adhere);
                    if(item.at(k+1) == finish ){
                        qDebug() << path;
                        if( !lenpath ){
                            shortPath = path;
                            lenpath = path.size();
                        }else if( path.size() <= lenpath ){
                            shortPath = path;
                            lenpath = path.size();
                        }
                        return;
                    }
                    shorestPaths(around,path);
                }
            }
        }
    }
}

QString
CalShortestPath::pointFindCity(QPointF pt)
{
    QString name = "";
    QMap<QString,QPointF>::iterator it = mCitys.begin();
    while( it != mCitys.end() ){
        if( it.value() == pt ){
            name = it.key();
            break;
        }
        it++;
    }
    return  name;
}

QPointF
CalShortestPath::cityFindPoint(QString name)
{
    QPointF point;
    foreach(QString item, mCitys.keys()){
        if( !item.compare(name,Qt::CaseInsensitive) ){
            point = mCitys[item];
            break;
        }
    }
    return  point;
}
