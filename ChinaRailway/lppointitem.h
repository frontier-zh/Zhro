#ifndef LPPOINTITEM_H
#define LPPOINTITEM_H

#include <QGraphicsEllipseItem>

class LPPointItem : public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    explicit LPPointItem(QString city, QPoint pt, QGraphicsEllipseItem *parent = 0);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF  boundingRect() const;
private:

    QString  name;
    QPoint   point;

};

#endif // LPPOINTITEM_H
