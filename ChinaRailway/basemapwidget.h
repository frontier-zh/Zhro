#ifndef BASEMAPWIDGET_H
#define BASEMAPWIDGET_H
#include "basepluginwidget.h"
#include <QGraphicsView>
#include <QDebug>

typedef QList<QList<QVariant> > RESULT;

class BaseMapWidget : public BasePluginWidget
{
public:
    explicit BaseMapWidget(QWidget *parent = 0);
protected:
    void    initUi();
    void    curState();
private:
    QGraphicsView   *gview;
    RESULT          pvdata;
    int            maxpv;
    QMap<QString,QPointF>    mCitys;
    QMap<QString,QPainterPath> mPolys;

};

#endif // BASEMAPWIDGET_H
