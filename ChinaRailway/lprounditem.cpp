#include "lprounditem.h"
#include <QPainter>
#include "public_define.h"
#include <QTime>



LPRoundItem::LPRoundItem(QString text, QPointF pt, QGraphicsItem *parent) :
    QGraphicsItem(parent), ishover(false)
{
    name = text;
    point = pt;
    this->setAcceptTouchEvents(true);
    this->setAcceptHoverEvents(true);
}

void LPRoundItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QRectF rt = this->boundingRect();
    painter->setPen(QColor(0x23, 0x56, 0x79));
    if( ishover ){
        painter->setBrush(QBrush(QColor(0xff, 0x00, 0x00, 255 * 1)));
    }else{
        painter->setBrush(QBrush(QColor(0xff, 0xff, 0xff, 255 * 1)));
    }
    painter->drawEllipse(rt);

    if( name.contains("1") ){
        QString scity = name;
        scity = scity.replace("1","");
        QFont font(FONT_NAME, 7);
        QFontMetricsF fm(font);
        QSizeF sz = fm.size(0, scity);

        qreal x = 0, y = 0;
        if( rt.right() > 520 ){
            x = rt.right();
            y = rt.top() + sz.height() / 2;
        }else{
            x = rt.left() - sz.width();
            y = rt.top() + sz.height() / 2;
        }
        painter->setFont(font);
        painter->setPen(QColor(0x00, 0x00, 0x00));
        painter->drawText(x, y, scity);
    }
}

void
LPRoundItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug() << __FUNCTION__;
    //setFocus();
    setToolTip(name);
}

void
LPRoundItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << __FUNCTION__;
    if( ishover ){
        ishover = false;
        emit    onReleaseRoundItem(name);
    }else{
        ishover = true;
        emit    onClickRoundItem(name);
    }
    setFocus();

}

QRectF LPRoundItem::boundingRect() const
{
    int padding = 4;
    QRectF _boundingRect(QPointF(point.x() - padding,point.y() - padding),
                         QPointF(point.x() + padding,point.y() + padding));
    return _boundingRect;
}
