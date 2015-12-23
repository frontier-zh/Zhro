#include "distancelatlng.h"
#include "ui_distancelatlng.h"
#include <qmath.h>
#include <QDebug>
#include "public_helper.h"

DistanceLatLng::DistanceLatLng(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DistanceLatLng), poly(""), PI(3.1415926), EARTH_RADIUS(6378137)
{
    ui->setupUi(this);
    ui->polylines->setToolTip("至少输入两点经纬度,也可以轮廓");
    ui->polylines->setStatusTip("至少输入两点经纬度,也可以轮廓");
    ui->result_2->setPlaceholderText("请输入地图指示坐标");
    ui->result->setEnabled(false);
    connect(ui->button, SIGNAL(clicked()), SLOT(lineDistance()));
    connect(ui->button_2, SIGNAL(clicked()), SLOT(obtainGoogleRealLatLng()));
}

DistanceLatLng::~DistanceLatLng()
{
    delete ui;
}

double
DistanceLatLng::dotDistance(QString dot1, QString dot2)
{
    if( !dot1.isEmpty() && dot1.contains(",") && !dot2.isEmpty() && dot2.contains(",") ){
        double lat1 = dot1.split(",").at(1).toDouble();
        double lng1 = dot1.split(",").at(0).toDouble();
        double lat2 = dot2.split(",").at(1).toDouble();
        double lng2 = dot2.split(",").at(0).toDouble();

        double radLat1 = rad(lat1);
        double radLat2 = rad(lat2);
        double a = radLat1 - radLat2;
        double b = rad(lng1) - rad(lng2);

        double s = 2 * qAsin(qSqrt(qPow(qSin(a/2),2) +
         qCos(radLat1)*qCos(radLat2)*qPow(qSin(b/2),2)));
        s = s * EARTH_RADIUS;
        s = qRound(s * 10000) / 10000;
        return s;
    }else{
        return 0;
    }
}

void
DistanceLatLng::lineDistance()
{
    QString poly = ui->polylines->toPlainText();
    if( poly.isEmpty() ){
        ui->polylines->setPlainText("输入至少两点经纬度!");
        return;
    }
    double  totlen = 0.0;
    QStringList  allpart = poly.split("\",\"");
    foreach( QString part, allpart ){
        part = part.replace("\"","");
        part = part.trimmed();
        QStringList alldot = part.split(";");
        if( alldot.size() >= 2 ){
            for( int i = 0; i < alldot.size()-1; i++ ){
                QString  dot1 = alldot.at(i);
                QString  dot2 = alldot.at(i+1);
                double   len = dotDistance(dot1,dot2);
                totlen += len;
            }
        }
    }
    ui->result->setText("距离是: " + QString::number(totlen) + "米");
    return  ;
}

void
DistanceLatLng::obtainGoogleRealLatLng()
{
    QString loc = ui->result_2->text();
    qDebug() << "fake location [google/amap]:" << loc;
    if( !loc.isEmpty() && loc.contains(",") ){
        loc.replace("\"","");
        QString signalloc = loc.split(";").at(0);
        Coordinate  cor;
        Coordinate  cor1;
        cor.lng = signalloc.split(",").at(0).toDouble();
        cor.lat = signalloc.split(",").at(1).toDouble();
        cor1 = fakeLatLngToRealLatLng(cor);
        loc = QString::number(cor1.lng,'f',6) + "," + QString::number(cor1.lat,'f',6);
        qDebug() << "real location [google/amap]:" << loc;
    }

    this->ui->result_3->setText(loc);
}
