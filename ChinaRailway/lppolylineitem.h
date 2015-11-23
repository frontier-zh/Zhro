#ifndef LPPOLYLINEITEM_H
#define LPPOLYLINEITEM_H

#include <QGraphicsPathItem>

class LPPolylineItem : public QObject,public QGraphicsPathItem
{
    Q_OBJECT
public:
    explicit LPPolylineItem(QGraphicsItem * parent = 0);

signals:

public slots:

};

#endif // LPPOLYLINEITEM_H
