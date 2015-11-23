#include "flowdatastate.h"
#include "public_define.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsEllipseItem>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QGraphicsScene>
#include <QTimer>
#include <QDebug>
#include <QPen>
#include <QDateTime>
#include "lprounditem.h"
#include "lppolylineitem.h"
#include "shortestpaththread.h"

FlowDataState::FlowDataState(QObject *parent) :
    MapViewState( parent), begin(""), end(""), isShow(false)
{
    texts.clear();
    shortPath.clear();
    _timer = new QTimer(this);
    QObject::connect(_timer, SIGNAL(timeout()), this, SLOT(onTimerUpdatepage()));
}

void
FlowDataState::initCustomItems()
{
    text = new LPTextItem();
    QFont font(FONT_NAME, 9);
    font.setPixelSize(20);
    text->setFont(font);
    text->setDefaultTextColor(QColor(0xDC, 0x41, 0x31));
    text->setPos(QPointF(10.0, 20.0));
    text->setPlainText("China railway");
    scene->addItem(text);

}


inline QPropertyAnimation * inanimation(QGraphicsObject *item, int index, QObject *parent)
{
    item->setScale(0.0);
    QPropertyAnimation *ani = new QPropertyAnimation(item, "scale", parent);
    ani->setStartValue(0.0);
    ani->setEndValue(1.0);
    ani->setDuration(350 + 15 * index);
    ani->setEasingCurve(QEasingCurve::OutBack);

    return ani;
}

void
FlowDataState::setData(const QMap<QString,QPointF> &citys, const QMap<QString, QPainterPath> &paths)
{

    //描轨迹
    foreach( QString key, paths.keys() ){
        QPainterPath path = paths[key];
        LPPolylineItem *ite = new LPPolylineItem();
        QPen  pen = QPen(QColor(key));
        pen.setWidth(5);
        ite->setPen(pen);
        ite->setPath(path);
        scene->addItem(ite);
        mapItems[key] = ite;
    }
    //描圆点
    foreach( QString key, citys.keys() ){
        QPointF pt = citys[key];
        LPRoundItem *ite = new LPRoundItem(key,pt);
        connect(ite, SIGNAL(onClickRoundItem(QString)), this, SLOT(onResponeAddItem(QString)));
        connect(ite, SIGNAL(onReleaseRoundItem(QString)), this, SLOT(onResponeDelItem(QString)));
        scene->addItem(ite);
        mapItems[key] = ite;
    }
}

void
FlowDataState::loadThreadFinished()
{
    ShortestPathThread *thread = qobject_cast<ShortestPathThread*>(sender());
    shortPath = thread->getShortest();
    qDebug() << __FUNCTION__ << "thread respone short path :" << shortPath.size();
    if (!shortPath.size()) return;

    if( !_timer->isActive() ){
        _timer->start(500);
    }
}

void
FlowDataState::onResponeAddItem(QString name)
{
    qDebug() << "add city:" << name;
    if( begin.isEmpty() ){
        begin = name.replace("1","");
    }else if( end.isEmpty() ){
        end = name.replace("1","");
    }
    if( !begin.isEmpty() && !end.isEmpty() ){
        qDebug() << __FUNCTION__ << "call async thread short path ";
        ShortestPathThread *thread = new ShortestPathThread(this);
        thread->setCityname(begin, end);
        connect(thread, SIGNAL(finished()), this, SLOT(loadThreadFinished()));
        thread->start();
    }
}

void
FlowDataState::onResponeDelItem(QString name)
{
    qDebug() << "del city:" << name;
    if( name.contains(begin) ){
        begin = "";
    }
    if( name.contains(end) ){
        end = "";
    }
    if( begin.isEmpty() || end.isEmpty() ){
        if( _timer->isActive() ){
            _timer->stop();
            shortPath.clear();
        }
        LPPolylineItem *pli = (LPPolylineItem *)mapItems["show"];
        if( pli ){
            scene->removeItem(pli);
            mapItems.remove("show");
        }
    }
}

void
FlowDataState::onTimerUpdatepage()
{
    //画轨迹 或者
    LPPolylineItem *pli = (LPPolylineItem *)mapItems["show"];
    if( pli ){
        scene->removeItem(pli);
        mapItems.remove("show");
    }
    if( isShow ){
        isShow = false;
    }else{
        isShow = true;
        QPainterPath path;
        for( int i = 0; i < shortPath.size(); i++ ){
            QPointF  pt = shortPath.at(i);
            if( i == 0 ){
                path.moveTo(pt);
            }else{
                path.lineTo(pt);
            }
        }
        LPPolylineItem *ite = new LPPolylineItem();
        QPen  pen = QPen(QColor(Qt::black));
        pen.setWidth(3);
        ite->setPen(pen);
        ite->setPath(path);
        scene->addItem(ite);
        mapItems["show"] = ite;
    }

}
