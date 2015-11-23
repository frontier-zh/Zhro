#include "shortestpaththread.h"

ShortestPathThread::ShortestPathThread(QObject *parent) :
    QThread(parent), lenpath(0)
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
}

void
ShortestPathThread::run()
{
    QPointF begin = cityFindPoint(bname);
    QPointF end = cityFindPoint(ename);
    if( !allPoints.contains(begin) || !allPoints.contains(end) ){
        qDebug() << "another one point isn't contain in allPoints";
        return;
    }
    finish = end;

    QList<QPointF> lastaround;
    lastaround.append(begin);
    shorestPaths(lastaround,pointFindCity(begin));
    qDebug() << "task execute finished";
}

void
ShortestPathThread::shorestPaths(QList<QPointF> lastaround, QString path)
{
    //qDebug() << "[" << path << "]";
    QString  currPath = path;
    int i = 0;
    if( lenpath && lastaround.size() > lenpath ){
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
                    //qDebug() << path;
                    if( !lenpath ){
                        shortPath = around;
                        lenpath = around.size();
                    }else if( around.size() <= lenpath ){
                        shortPath = around;
                        lenpath = around.size();
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
                    //qDebug() << path;
                    if( !lenpath ){
                        shortPath = around;
                        lenpath = around.size();
                    }else if( around.size() <= lenpath ){
                        shortPath = around;
                        lenpath = around.size();
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
                        //qDebug() << path;
                        if( !lenpath ){
                            shortPath = around;
                            lenpath = around.size();
                        }else if( around.size() <= lenpath ){
                            shortPath = around;
                            lenpath = around.size();
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
                        //qDebug() << path;
                        if( !lenpath ){
                            shortPath = around;
                            lenpath = around.size();
                        }else if( around.size() <= lenpath ){
                            shortPath = around;
                            lenpath = around.size();
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
ShortestPathThread::pointFindCity(QPointF pt)
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
ShortestPathThread::cityFindPoint(QString name)
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

