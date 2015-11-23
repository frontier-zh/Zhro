#ifndef LPTEXTITEM_H
#define LPTEXTITEM_H

#include <QGraphicsObject>

class LPTextItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    explicit LPTextItem(QGraphicsItem *parent = 0);

    void setLineaGridentTextColor(const QColor& start, const QColor& end);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    QColor _start, _end;
};

#endif // LPTEXTITEM_H
