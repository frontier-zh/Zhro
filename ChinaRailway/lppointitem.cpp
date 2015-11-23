#include "lppointitem.h"
#include <QPainter>

LPPointItem::LPPointItem(QString text, QPoint pt, QGraphicsEllipseItem *parent) :
    QGraphicsEllipseItem(parent)
{
    name = text;
    point = pt;
}

QRectF
LPPointItem::boundingRect() const
{
    QRectF _boundingRect(QPointF(point.x()-10,point.y()-10),QPointF(point.x()+10,point.y()+10));
    return _boundingRect;
}

void
LPPointItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QRectF rt = this->boundingRect();
        painter->setPen(QColor(0x66, 0x66, 0x66));
        painter->setBrush(QBrush(QColor(0xFF, 0xFF, 0xFF, 255 * 0.5)));
        painter->drawEllipse(rt);

//        QFont font(FONT_NAME, 9);
//        font.setPixelSize(12);

//        drawGlorryText(painter, name, font,
//                       rt, QColor(0xFF, 0xFF, 0xFF, 200),
//                       QColor(0x66, 0x66, 0x66));


    painter->restore();
}
