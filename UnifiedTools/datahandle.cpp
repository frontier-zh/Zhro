#include "datahandle.h"

DataHandle::DataHandle(QObject *parent) :
    QObject(parent)
{
    exp_host.clear();
    QFile file("config/host.txt");
    if( file.exists() ){
        if( file.open(QIODevice::ReadOnly|QIODevice::Text)){
            QTextStream  in(&file);
            QString  str = "";
            while(!(str = in.readLine()).isEmpty()){
                exp_host.append(str.trimmed());
            }
            file.close();
        }
    }

    QSettings setting("config/config.ini", QSettings::IniFormat);

    imp_host = setting.value("import/host","").toString().split(" ");
    imp_name = setting.value("import/name","").toString().split(" ");
    imp_user = setting.value("import/user","").toString().split(" ");
    imp_pass = setting.value("import/pass","").toString().split(" ");
    exp_name = setting.value("export/name","").toString().split(" ");
    exp_user = setting.value("export/user","").toString().split(" ");
    exp_pass = setting.value("export/pass","").toString().split(" ");
}
DataHandle&
DataHandle::instance()
{
    static DataHandle _inst;
    return _inst;
}

QStringList &
DataHandle::getHostList()
{
    return  this->exp_host;
}

QStringList &
DataHandle::getImpHostList()
{
    return  this->imp_host;
}

QStringList &
DataHandle::getImpDbList()
{
    return  this->imp_name;
}

QStringList &
DataHandle::getImpUserList()
{
    return  this->imp_user;
}

QStringList &
DataHandle::getImpPassList()
{
    return  this->imp_pass;
}

QStringList &
DataHandle::getExpHostList()
{
    return  this->exp_host;
}

QStringList &
DataHandle::getExpDbList()
{
    return  this->exp_name;
}

QStringList &
DataHandle::getExpUserList()
{
    return  this->exp_user;
}

QStringList &
DataHandle::getExpPassList()
{
    return  this->exp_pass;
}
