#include "lptextitem.h"
#include <QPainter>

LPTextItem::LPTextItem(QGraphicsItem *parent) :
    QGraphicsTextItem(parent)
{
    this->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

QRectF
LPTextItem::boundingRect() const
{
    QFontMetricsF fm(this->font());
    QSizeF sz = fm.size(0, this->toPlainText());

    return QRectF(0.0, 0.0, sz.width() + 20, sz.height() * 1.5);
}

void
LPTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing, true);

    QFont font(this->font());
    QString text = this->toPlainText();

    QFontMetricsF fm(font);

    qreal x = 5;
    qreal y = 5 + fm.ascent();

    QPainterPath path;
    path.addText(x, y, font, text);

    QPainterPathStroker pps;
    pps.setCapStyle(Qt::RoundCap);
    pps.setJoinStyle(Qt::RoundJoin);
    pps.setWidth(11);

    QPainterPath bp = pps.createStroke(path);
    painter->fillPath(bp, QBrush(QColor(Qt::white)));

    if (_start.isValid()) {
        QLinearGradient lg(0.0, 5, 0.0, fm.ascent() + fm.descent());
        lg.setColorAt(0.0, _start); // QColor(0xFB, 0x70, 0x70));
        lg.setColorAt(1.0, _end);   // QColor(0x6D, 0x11, 0x0D));

        painter->fillPath(path, QBrush(lg));
    } else {
        painter->setFont(font);
        painter->setPen(this->defaultTextColor());
        painter->drawText(x, y, text);
    }

    painter->restore();
}

void
LPTextItem::setLineaGridentTextColor(const QColor &start, const QColor &end)
{
    _start = start;
    _end = end;
}
