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

CVS_Fileparse::CVS_Fileparse(QList<QStringList> fields, bool isMulti)
{
    this->ismulti = isMulti;
    this->fields = fields;
    QDir  dir(".");
    QStringList filter("*.CSV");
    dir.setNameFilters(filter);
    if( dir.exists()){
        files = dir.entryList(QDir::Files|QDir::NoDotAndDotDot,QDir::Name);
    }
}
void CVS_Fileparse::parseFilecontext()
{
    QList<QList<RESULT> > results;
    foreach(QString sfname, files)
    {
        QString dfname = QString(sfname.split(".").at(0) + ".xlsx");
        QFile  sfile(sfname);
        QList<RESULT> result;

        if(sfile.exists()){
            if( sfile.open(QIODevice::ReadOnly|QIODevice::Text)){
                QTextStream  in(&sfile);
                QString buff= in.readAll();
                int  begin = 0;
                int  end = 0;

                foreach(QStringList item, fields){
                    int   pos = 0;
                    RESULT pair;
                    QString buff1, buff2, buff3;
                    begin = buff.indexOf(item.at(0),end);
                    if( begin == -1){
                        begin = lastbegin;
                    }else{
                        lastbegin = begin;
                    }
                    end   = buff.indexOf(item.at(1),begin);
                    buff1  = buff.mid(end);
                    buff1 = buff1.split("\n").at(0);
                    pos = item.at(3).toInt();
                    buff2 = buff1.split(",").at(pos+1);
                    buff3 = buff1.split(",").at(pos+2);
                    if( buff3.contains(".")){
                        buff3 = buff3.mid(0,buff3.indexOf(".")+3);
                    }
                    if( item.at(1).contains("位置度")){
                        pair.first = item.at(0).split("：").at(1) + item.at(1) + buff3;
                    }else{
                        pair.first = item.at(0).split("：").at(1) + item.at(1);
                    }

                    pair.second = buff2;
                    result.append(pair);
                }
                sfile.close();
            }
        }
        if( this->ismulti){
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
        results.append(result);
    }
    if( !this->ismulti){
        QString dfname = QString("三坐标数据汇总.xlsx");
        QXlsx::Document xlsx;
        for(int i=0; i<files.size(); i++ ){
            QList<RESULT> item = results.at(i);
            QString title = files.at(i).split(".").at(0);
            QChar   t(65);
            QString head = QString(t) + QString::number(i+2);
            xlsx.write(head, title);
            for(int j=0; j<item.size(); j++ ){
                QChar c(66+j);
                if( i==0 ){
                    QString key1 = QString(c) + "1";
                    xlsx.write(key1, item.at(j).first);
                }
                QString key2 = QString(c) + QString::number(i+2);
                xlsx.write(key2, item.at(j).second);
            }
        }
        xlsx.saveAs(dfname);
    }
}
