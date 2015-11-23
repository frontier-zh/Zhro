#ifndef BASEPLUGINWIDGET_H
#define BASEPLUGINWIDGET_H

#include <QWidget>
#include <QPixmap>

class BasePluginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BasePluginWidget(QWidget *parent = 0);
protected:
    virtual  void  initUi()=0;
signals:

public slots:

};

#endif // BASEPLUGINWIDGET_H
