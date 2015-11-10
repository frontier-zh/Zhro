#include "drawpolygon.h"
#include "ui_drawpolygon.h"
#include <QMessageBox>
#include <QDebug>
#include <QPainter>


DrawPolygon::DrawPolygon(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DrawPolygon), scene(NULL)
{
    ui->setupUi(this);
    ui->polygon->setToolTip("请输入多个(x,y)点,绘制轮廓");
    connect(ui->button, SIGNAL(clicked()), SLOT(drawMutliPolygon()));
}

DrawPolygon::~DrawPolygon()
{
    delete ui;
}

void
DrawPolygon::drawMutliPolygon()
{
    if( scene ){
        scene->deleteLater();
        scene = NULL;
    }
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    QRect  vRec = ui->graphicsView->rect();
    int    padding = 30;
    int    top = vRec.top() + padding;
    int    left = vRec.left() + padding;
    int    bottom = vRec.bottom() - padding;
    int    right = vRec.right() - padding;


        poly = ui->polygon->toPlainText();
        if( poly.isEmpty() ){
            QMessageBox::warning(this,"注意","绘制轮廓点不能为空",NULL,NULL);
            return;
        }
        if( poly.contains(",0 ") ){
            poly.replace(",0 ",";");
        }
        poly.replace("\"","");
        QStringList  points = poly.split(";");
        QList<QPointF> list;
        double minlat = 90.0;
        double maxlat = 0.0;
        double minlng = 180.0;
        double maxlng = 0.0;
        foreach( QString item, points){
            double lng = item.split(",").at(0).toDouble();
            double lat = item.split(",").at(1).toDouble();
            if( lng > maxlng ) maxlng = lng;
            if( lng < minlng ) minlng = lng;
            if( lat > maxlat ) maxlat = lat;
            if( lat < minlat ) minlat = lat;
            list.append(QPointF(lng,lat));
        }

        _path = new QGraphicsPathItem();
        _path->setPen(QPen(QBrush(Qt::black),2));
        QPainterPath path;
        path.moveTo(getPlaneCoordinate(minlng, maxlng, minlat, maxlat,top,bottom,left,right,list.at(0)));
        for( int i = 1; i < list.size(); i++ ){
            path.lineTo(getPlaneCoordinate(minlng, maxlng, minlat, maxlat,top,bottom,left,right,list.at(i)));
        }
        _path->setPath(path);

    scene->addItem(_path);


}


QPointF
DrawPolygon::getPlaneCoordinate(double minlng, double maxlng, double minlat, double maxlat,
                                int top, int bottom, int left, int right, QPointF cor)
{
    if( (maxlng - minlng) / (maxlat - minlat) <= 2 ){
        double x = ((cor.x() - minlng) * (bottom - top) / (maxlat - minlat)) + left;
        double y = ((cor.y() - maxlat) * (bottom - top) / (minlat - maxlat)) + top;
        return QPointF(x,y);
    }else{
        double x = ((cor.x() - minlng) * (right - left) / (maxlng - minlng)) + left;
        double y = ((cor.y() - maxlat) * (right - left) / (minlng - maxlng)) + top;
        return QPointF(x,y);
    }
}
