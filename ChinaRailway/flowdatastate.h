#ifndef FLOWDATASTATE_H
#define FLOWDATASTATE_H
#include "mapviewstate.h"


class FlowDataState : public MapViewState
{
    Q_OBJECT
public:
    explicit FlowDataState( QObject *parent = NULL);

    void setData(const QMap<QString, QPointF> &citys,
                 const QMap<QString, QPainterPath> &paths);

protected:
    void    initCustomItems();
protected slots:
    void    loadThreadFinished();
    void    onResponeAddItem(QString name);
    void    onResponeDelItem(QString name);
    void    onTimerUpdatepage();

protected:
    QList<LPTextItem*>  texts;
    QList<QPointF>      shortPath;
    QString     begin;
    QString     end;
    QTimer      *_timer;
    bool        isShow;
};

#endif // FLOWDATASTATE_H
