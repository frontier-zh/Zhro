#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include "urlencode.h"
#include "unicodetozh.h"
#include "timestamp.h"
#include "httpanalyse.h"
#include "public_define.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), dl(NULL)
{
    ui->setupUi(this);

    QPixmap pixmap(":/image/0000.png");
    palette.setBrush(QPalette::Window, QBrush(pixmap));

    QList<QString>  titles;
    titles.append("Http访问");
    titles.append("UrlEncode编码");
    titles.append("Unicode转中文");
    titles.append("时间戳转换");
    titles.append("Http请求标头");
    titles.append("待添加");
    titles.append("待添加-1");
    titles.append("待添加-2");
    titles.append("待添加-3");
    titles.append("待添加-4");
    titles.append("待添加-5");
    titles.append("待添加-6");

    char    buff[100] = {0};
    for(int i=0; i < titles.size(); i++){
        QString title = titles.at(i);
        sprintf(buff,":/image/%d.png", i);
        QListWidgetItem* lst = new QListWidgetItem(QIcon(buff), title, ui->listWidget);
        //设置QListWidgetItem高度.
        lst->setSizeHint(QSize(lst->sizeHint().height(),40));
        ui->listWidget->insertItem(i, lst);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

//可以将这些 按钮点击事件 手动connect()到同一个槽函数,减少代码冗余.


void MainWindow::on_listWidget_doubleClicked(const QModelIndex &index)
{
    qDebug() << "idx: " << index.row();
    int type = index.row();
    if( dl ){
        ui->horizontalLayout->removeWidget(dl);
        dl->deleteLater();
        dl = NULL;
    }
    if( type == 0 ){
        dl = new Dialog(this);
    }else if( type == 1 ){
        dl = new UrlEncode(this);
    }else if( type == 2 ){
        dl = new UnicodeTozh(this);
    }else if( type == 3 ){
        dl = new TimeStamp(this);
    }else if( type == 4 ){
        dl = new HttpAnalyse(this);
    }else {
        return;
    }
    if( dl )
    {
        //dl->setStyleSheet("QPlainTextEdit#cookiestring{ background-color:blue}"); //针对全部
        //dl->setStyleSheet("QPlainTextEdit { background-color:blue}"); //针对具体某个实例
        dl->setAutoFillBackground(true);
        dl->setPalette(palette);
        dl->show();
        ui->horizontalLayout->addWidget(dl);
        ui->statusBar->showMessage(QString("%1. ").arg(type+1) + index.data(0).toString(), 10000);
    }

}
