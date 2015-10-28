#include "datacomparisonthread.h"

DataComparisonThread::DataComparisonThread(QObject *parent) :
    QThread(parent)
{
}

void DataComparisonThread::run()
{
    MODEL  smodel;
    MODEL  tmodel;
    match = false;
    total = 0;
    result.clear();

    TaskSourceDatabase db1;
    QStringList data;
    QLOG_TRACE() << "DataComparison Connecting to source" << infos[0].host;
    if (!db1.connectToDB(infos[0].host,
                            infos[0].port,
                            infos[0].dbname,
                            infos[0].tasktablename,
                            infos[0].patterntablename,
                            infos[0].uid,
                            infos[0].pwd,
                            infos[0].condstring,
                            infos[0].datasource))
    {
        QString result = QString("旧系统房源库链接失败[ %1 : %2 : %3 : %4 : %5 ]")
                    .arg(infos[0].host).arg(infos[0].dbname).arg(infos[0].port).arg(infos[0].uid).arg(infos[0].pwd);
        QLOG_TRACE() << result;
        emit stopFeekback(result);
        return;
    }

    TaskSourceDatabase db2;
    QLOG_TRACE() << "DataComparison Connecting to target" << infos[1].host;
    if (!db2.connectToDB(infos[1].host,
                            infos[1].port,
                            infos[1].dbname,
                            infos[1].tasktablename,
                            infos[1].patterntablename,
                            infos[1].uid,
                            infos[1].pwd,
                            infos[1].condstring,
                            infos[1].datasource))
    {
        QString result = QString("新系统房源库链接失败[ %1 : %2 : %3 : %4 : %5 ]")
                    .arg(infos[1].host).arg(infos[1].dbname).arg(infos[1].port).arg(infos[1].uid).arg(infos[1].pwd);
        QLOG_TRACE() << result;
        emit stopFeekback(result);
        db1.closeDB();
        return;
    }
    if (!db1.loadDatagetOrigin(smodel)){
        QString result = QString("旧系统获得房源记录失败[ %1 : %2 ]").arg(infos[0].host).arg(infos[0].dbname);
        QLOG_TRACE() << result;
        emit stopFeekback(result);
        db1.closeDB();
        db2.closeDB();
        return;
    }
    if (!db2.loadDatagetOrigin(tmodel)){
        QString result = QString("新系统获得房源记录失败[ %1 : %2 ]").arg(infos[1].host).arg(infos[1].dbname);
        QLOG_TRACE() << result;
        emit stopFeekback(result);
        db1.closeDB();
        db2.closeDB();
        return;
    }
    db1.closeDB();
    db2.closeDB();

    float   percent;
    int slen = smodel.size();
    int tlen = tmodel.size();
    QLOG_TRACE() << "Source model size: " << slen << " Target model size: " << tlen;
    if( !(slen && tlen) ){
        QString result = QString("新[ %1 ]、旧[ %2 ]  无法完成比对!").arg(tlen).arg(slen);
        QLOG_TRACE() << result;
        emit stopFeekback(result);
        return;
    }

    for( int i=0; i < tlen; i++ ){
        QMap<QString,QVariant> tmap = tmodel.at(i);
        QString turl = tmap["sourceurl"].toString();

        for( int h=0; h < slen; h++ ){
            QMap<QString,QVariant> smap = smodel.at(h);
            QString surl = smap["sourceurl"].toString();
            if( turl.compare(surl) ){
                continue;
            }else if( tmap["influxtime"].isNull() && smap["influxtime"].isNull()){
                match = true;
                total++;
                percent = i * 1.0 / tlen;
                QStringList keys = tmap.keys();
                keys.removeOne("id");
                keys.removeOne("gettime");
                keys.removeOne("influxtime");
                foreach( QString key, keys ){
                    if( tmap.value(key).toString().compare(smap.value(key).toString()) ){
                        data.append(tmap["sourceurl"].toString());
                        data.append(key);
                        data.append("否");
                        data.append(tmap.value(key).toString());
                        data.append(smap.value(key).toString());
                        data.append(tmap["gettime"].toString());
                        data.append(smap["gettime"].toString());
                        //data.append(QString("%1%").arg(qRound(percent*100)));
                        result.append(data);
                        data.clear();
                    }
                }
                break;
            }else if( !tmap["influxtime"].isNull() && !smap["influxtime"].isNull()){
                match = true;
                total++;
                percent = i * 1.0 / tlen;
                QStringList keys = tmap.keys();
                keys.removeOne("id");
                keys.removeOne("gettime");
                keys.removeOne("influxtime");
                foreach( QString key, keys ){
                    if( tmap.value(key).toString().compare(smap.value(key).toString()) ){
                        data.append(tmap["sourceurl"].toString());
                        data.append(key);
                        data.append("是");
                        data.append(tmap.value(key).toString());
                        data.append(smap.value(key).toString());
                        data.append(tmap["gettime"].toString());
                        data.append(smap["gettime"].toString());
                        result.append(data);
                        data.clear();
                    }
                }
                break;
            }
        }
    }
    if( !match ){
        QString feedback = QString("新[ %1 ]、旧[ %2 ]  未找到相同URL的匹配记录!").arg(tlen).arg(slen);
        QLOG_TRACE() << feedback;
        emit stopFeekback(feedback);
    }else{
        QString feedback = QString("新[ %1 ]、旧[ %2 ]  找到相同URL记录数[ %3 ]条. 差异字段共[ %4 ]个.").arg(tlen).arg(slen).arg(total).arg(result.size());
        QLOG_TRACE() << feedback;
        emit stopFeekback(feedback);
    }
}
