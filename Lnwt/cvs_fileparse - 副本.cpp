#include "cvs_fileparse.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QtXlsx>

CVS_Fileparse::CVS_Fileparse(QObject *parent) :
    QObject(parent)
{
}

CVS_Fileparse::CVS_Fileparse(QList<QStringList> fields)
{
    this->fields = fields;
    foreach(QStringList field, fields){
        qDebug() << field;
    }

    QDir  dir(".");
    QStringList filter("*.CSV");
    dir.setNameFilters(filter);
    if( dir.exists()){
        files = dir.entryList(QDir::Files|QDir::NoDotAndDotDot);
    }
    qDebug() << files;
}
void CVS_Fileparse::parseFilecontext()
{
    foreach(QString sfname, files)
    {
        QString dfname = QString(sfname.split(".").at(0) + ".xlsx");
        QFile  sfile(sfname);
        qDebug() << dfname;
        QList<QPair<QString,QString> > result;
        if(sfile.exists()){
            if( sfile.open(QIODevice::ReadOnly|QIODevice::Text)){
                QTextStream  in(&sfile);
                QString context = in.readAll();
                foreach(QStringList item, fields){
                    qDebug() << "zhro:" << item.size();
                    QString buff(context);
                    QString buff1, buff2, buff3;
                    int  pos_begin = buff.indexOf(item.at(0));
                    int  pos_end = buff.indexOf(item.at(1),pos_begin);
                    buff = buff.mid(pos_end);
                    qDebug() << "zhro";
                    buff1 = buff.split("\n").at(0);
                    qDebug() << "zhro1:" << buff1;
                    int  pos = item.at(3).toInt();
                    qDebug() << "zhro2:" << pos;
                    buff2 = buff1.split(",").at(pos+1);
                    buff3 = buff1.split(",").at(pos+2);
                    QPair<QString,QString> pair;
                    if( item.at(1).contains("位置度")){
                        pair.first = item.at(0).split("：").at(1) + item.at(1) + buff3;
                    }else{
                        pair.first = item.at(0).split("：").at(1) + item.at(1);
                    }

                    pair.second = buff2;
                    result.append(pair);
                    qDebug() << pair.first << ":" << pair.second;
                }
                sfile.close();
            }
        }

        QXlsx::Document xlsx;
        for(int i=0; i<result.size(); i++ ){
            QChar c(65+i);
            QString key1 = QString(c) + "1";
            QString key2 = QString(c) + "2";
            xlsx.write(key1, result.at(i).first);
            xlsx.write(key2, result.at(i).second);
        }
        xlsx.saveAs(dfname);
    }
}
