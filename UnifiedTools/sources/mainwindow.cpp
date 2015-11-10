#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include "urlencode.h"
#include "unicodetozh.h"
#include "timestamp.h"
#include "httpanalyse.h"
#include "taskimport.h"
#include "drawpolygon.h"
#include "datacomparison.h"
#include "distancelatlng.h"
#include "refinelatlng.h"
#include "tcpclisocket.h"
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
    titles.append("采集任务导入");
    titles.append("房源信息对比");
    titles.append("地图使用");
    titles.append("多点绘制轮廓");
    titles.append("经纬度细化");
    titles.append("TCP/IP[Cli_Send]");
    titles.append("待添加-1");
    titles.append("待添加-2");
    titles.append("待添加-3");
    titles.append("待添加-4");

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
        QLOG_TRACE() << "------------HTTP访问测试-------------";
        dl = new Dialog(this);
    }else if( type == 1 ){
        QLOG_TRACE() << "------------UrlEncode编码-------------";
        dl = new UrlEncode(this);
    }else if( type == 2 ){
        QLOG_TRACE() << "------------Unicode转中文-------------";
        dl = new UnicodeTozh(this);
    }else if( type == 3 ){
        QLOG_TRACE() << "------------时间戳转换-------------";
        dl = new TimeStamp(this);
    }else if( type == 4 ){
        QLOG_TRACE() << "------------HTTP访问测试-------------";
        dl = new HttpAnalyse(this);
    }else if( type == 5 ){
        QLOG_TRACE() << "------------任务导入模块-------------";
        dl = new TaskImport(this);
    }else if( type == 6 ){
        QLOG_TRACE() << "------------房源信息对比-------------";
        dl = new DataComparison(this);
    }else if( type == 7 ){
        QLOG_TRACE() << "------------地图使用-------------";
        dl = new DistanceLatLng(this);
    }else if( type == 8 ){
        QLOG_TRACE() << "------------轮廓绘制-------------";
        dl = new DrawPolygon(this);
    }else if( type == 9 ){
        QLOG_TRACE() << "------------经纬度范围细化-------------";
        dl = new RefineLatLng(this);
    }else if( type == 10 ){
        QLOG_TRACE() << "------------TCP/IP[Cli_Send]-------------";
        dl = new TcpCliSocket(this);
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
