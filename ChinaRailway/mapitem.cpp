#include "mapitem.h"
#include "public_define.h"
#include <QPainter>
#include <QtMath>


//inline void drawGlorryText(QPainter *painter, const QString& text,
//                           const QFont &font, const QColor &gcolor, const QColor &textColor,
//                           bool linebreak = false)
//{
//    Q_UNUSED(gcolor);
//    QFontMetricsF fm(font);
//    QSizeF sz = fm.size(0, text);

//        QString sub = text.left(2);
//        QString left = text.right(text.length() - 2);

//        QSizeF sz = fm.size(0, sub);
//        qreal x1 = rt.left() + (rt.width() - sz.width()) / 2.0;
//        qreal y1 = rt.top() + (rt.height() - fm.lineSpacing() - sz.height()) / 2.0 + fm.ascent();

//        sz = fm.size(0, left);
//        qreal x2 = rt.left() + (rt.width() - sz.width()) / 2.0;
//        qreal y2 = y1 + fm.lineSpacing();
//        painter->setFont(font);
//        painter->setPen(textColor);
//        painter->drawText(x1, y1, sub);
//        painter->drawText(x2, y2, left);
//}



void
MapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

        painter->setPen(QColor(0x66, 0x66, 0x66));
        painter->setBrush(QBrush(QColor(0xFF, 0xFF, 0xFF, 255 * 0.5)));
        painter->drawEllipse(point.x(),point.y(),20,20);

//        QFont font(FONT_NAME, 9);
//        font.setPixelSize(12);

//        drawGlorryText(painter, name, font,
//                       rt, QColor(0xFF, 0xFF, 0xFF, 200),
//                       QColor(0x66, 0x66, 0x66));


    painter->restore();
}
