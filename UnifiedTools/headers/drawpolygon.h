#ifndef DRAWPOLYGON_H
#define DRAWPOLYGON_H

#include <QDialog>
#include <QGraphicsPathItem>

namespace Ui {
class DrawPolygon;
}

class DrawPolygon : public QDialog
{
    Q_OBJECT

public:
    explicit DrawPolygon(QWidget *parent = 0);
    ~DrawPolygon();
    QPointF getPlaneCoordinate(double,double,double,double,
                               int,int,int,int,
                               QPointF);
public slots:
    void    drawMutliPolygon();

private:
    Ui::DrawPolygon *ui;
    QString         poly;
    QGraphicsPathItem   *_path;
    QGraphicsScene      *scene;
    QList<QPointF>  list;
};

#endif // DRAWPOLYGON_H
