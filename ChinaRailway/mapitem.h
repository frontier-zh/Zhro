#ifndef MapItem_H
#define MapItem_H

#include <QGraphicsItem>
#include <QtCore>
#include <QString>
#include <QPoint>

class MapItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit MapItem(QString city, QPoint pt, QGraphicsItem * parent = 0);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:

    QString  name;
    QPoint   point;
};

#endif // MapItem_H
