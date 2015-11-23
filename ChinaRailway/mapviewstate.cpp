#include "mapviewstate.h"
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QGraphicsScene>

MapViewState::MapViewState(QObject *parent) :
    QObject(parent)
{
    mapItems.clear();
}

MapViewState::~MapViewState()
{
    Q_FOREACH(QGraphicsItem *item, mapItems.values()) {
        SAFE_DELETE(item);
    }
    mapItems.clear();
    SAFE_DELETE(poly);
    SAFE_DELETE(text);
}

void
MapViewState::init(QGraphicsScene *scene)
{
    this->scene = scene;

    this->initCustomItems();

    //this->performInAnimation();
}

void
MapViewState::performInAnimation()
{
    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);


    if (text) {
        QPointF pt = text->pos();
        QPointF start(-text->boundingRect().width(), pt.y());
        text->setPos(start);

        group->addAnimation(this->inAnimation(text, start, pt));
    }

    if (poly) {
        QPointF pt = poly->pos();

        QPointF start(-poly->boundingRect().width(), pt.y());
        poly->setPos(start);
        group->addAnimation(this->inAnimation(poly, start, pt));
    }

    group->start(QAbstractAnimation::DeleteWhenStopped);

    connect(group, SIGNAL(finished()), this, SLOT(onInAnimationFinished()));
}

QPropertyAnimation*
MapViewState::inAnimation(QObject* obj, const QVariant &start, const QVariant &end)
{
    QPropertyAnimation *ani = new QPropertyAnimation(obj, "pos", this);
    ani->setStartValue(start);
    ani->setEndValue(end);
    ani->setDuration(500);
    ani->setEasingCurve(QEasingCurve::OutQuad);

    return ani;
}


void
MapViewState::onInAnimationFinished()
{

}
