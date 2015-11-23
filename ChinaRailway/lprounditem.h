#ifndef LPROUNDITEM_H
#define LPROUNDITEM_H

#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsSceneEvent>  //捕捉事件必须要引入

class LPRoundItem : public QObject,public QGraphicsItem
{
    Q_OBJECT
public:
    explicit LPRoundItem(QString text, QPointF pt, QGraphicsItem *parent = 0);

protected:
    void    paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void    hoverEnterEvent(QGraphicsSceneHoverEvent * event);

    //void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    //void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    //void hoverLeaveEvent (QGraphicsSceneHoverEvent * event);
    //void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    //void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    QRectF  boundingRect() const;
signals:
    void    onClickRoundItem(QString);
    void    onReleaseRoundItem(QString);
private:

    QString     name;
    QPointF     point;
    bool        ishover;
};

#endif // LPROUNDITEM_H
