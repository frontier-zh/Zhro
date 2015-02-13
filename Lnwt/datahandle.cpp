#include "datahandle.h"

DataHandle::DataHandle(QObject *parent) :
    QObject(parent)
{
    product.clear();
    QFile file("config/productkind.txt");
    if( file.exists() ){
        if( file.open(QIODevice::ReadOnly|QIODevice::Text)){
            QTextStream  in(&file);
            QString  str = "";
            while(!(str = in.readLine()).isEmpty()){
                product.append(str.trimmed());
            }
            file.close();
        }
    }

    standard.clear();
    QFile file1("config/productstandard.txt");
    if( file1.exists() ){
        if( file1.open(QIODevice::ReadOnly|QIODevice::Text)){
            QTextStream  in(&file1);
            QString  str = "";
            while(!(str = in.readLine()).isEmpty()){
                standard.append(str.trimmed());
            }
            file1.close();
        }
    }

    property.clear();
    QFile file2("config/procductproperty.txt");
    if( file2.exists() ){
        if( file2.open(QIODevice::ReadOnly|QIODevice::Text)){
            QTextStream  in(&file2);
            QString  str = "";
            while(!(str = in.readLine()).isEmpty()){
                QPair<QString,QString>  p;
                if( str.contains("#")){
                    p.first = str.split("#").at(0);
                    p.second = str.split("#").at(1);
                    property.append(p);
                }
            }
            file2.close();
        }
    }
}
DataHandle&
DataHandle::instance()
{
    static DataHandle _inst;
    return _inst;
}

QStringList &DataHandle::getProductList()
{
    return  this->product;
}

QStringList &DataHandle::getStandardList()
{
    return  this->standard;
}

QList<RESULT> &DataHandle::getPropertyList()
{
    return  this->property;
}
