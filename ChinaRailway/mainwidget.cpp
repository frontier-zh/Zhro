#include "mainwidget.h"
#include "basemapwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    initUi();
}

MainWidget::~MainWidget()
{

}
void
MainWidget::initUi()
{
    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->setSpacing(0);
    hbox->setMargin(0);

    //QScrollArea *scroll = new QScrollArea(this);
    //hbox->addWidget(scroll);

    QWidget *content = new QWidget(this);
    content->resize(960, 540);

    this->_layout = new QStackedLayout(content);
    this->_layout->setSpacing(0);
    this->_layout->setContentsMargins(0, 0, 0, 0);

    BaseMapWidget *map = new BaseMapWidget(this);
    this->_layout->addWidget(map);

    //scroll->setWidget(content);
    hbox->addWidget(content);
}
