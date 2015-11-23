#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QStackedLayout>

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);
    ~MainWidget();
protected:
    void  initUi();
private:
    QStackedLayout  *_layout;
};

#endif // MAINWIDGET_H
