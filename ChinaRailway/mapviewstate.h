#ifndef MAPVIEWSTATE_H
#define MAPVIEWSTATE_H
#include "lptextitem.h"
#include "lppolylineitem.h"
#include <QPropertyAnimation>
#include "public_define.h"


#include <QObject>

class MapViewState : public QObject
{

    Q_OBJECT
public:
    explicit MapViewState(QObject *parent = 0);
    virtual ~MapViewState();

    virtual void init(QGraphicsScene *scene);


public slots:
    void onInAnimationFinished();

protected:
    virtual void initCustomItems() = 0;
    void performInAnimation();

    QPropertyAnimation *inAnimation(QObject* obj,
                                    const QVariant& start, const QVariant& end);

protected:

    QGraphicsScene  *scene;
    LPTextItem      *text;
    LPPolylineItem  *poly;

    QMap<QString, QGraphicsItem*> mapItems;
    QList<QGraphicsItem*> listItems;

};

#endif // MAPVIEWSTATE_H
