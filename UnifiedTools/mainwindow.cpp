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
    QPixmap pixmap(":/image/1.png");
    palette.setBrush(QPalette::Window, QBrush(pixmap));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_button_clicked()
{
    if( dl ){
        ui->horizontalLayout->removeWidget(dl);
        dl->deleteLater();
        dl = NULL;
    }
    dl = new Dialog(this);
    dl->setAutoFillBackground(true);
    dl->setPalette(palette);
    dl->show();
    ui->horizontalLayout->addWidget(dl);
}

void MainWindow::on_button2_clicked()
{
    if( dl ){
        ui->horizontalLayout->removeWidget(dl);
        dl->deleteLater();
        dl = NULL;
    }
    dl = new UrlEncode(this);
    dl->setAutoFillBackground(true);
    dl->setPalette(palette);
    dl->show();
    ui->horizontalLayout->addWidget(dl);
}

void MainWindow::on_button3_clicked()
{
    if( dl ){
        ui->horizontalLayout->removeWidget(dl);
        dl->deleteLater();
        dl = NULL;
    }
    dl = new UnicodeTozh(this);
    dl->setAutoFillBackground(true);
    dl->setPalette(palette);
    dl->show();
    ui->horizontalLayout->addWidget(dl);
}

void MainWindow::on_button4_clicked()
{
    if( dl ){
        ui->horizontalLayout->removeWidget(dl);
        dl->deleteLater();
        dl = NULL;
    }
    dl = new TimeStamp(this);
    dl->setAutoFillBackground(true);
    dl->setPalette(palette);
    dl->show();
    ui->horizontalLayout->addWidget(dl);
}

void MainWindow::on_button5_clicked()
{
    if( dl ){
        ui->horizontalLayout->removeWidget(dl);
        dl->deleteLater();
        dl = NULL;
    }
    dl = new HttpAnalyse(this);
    dl->setAutoFillBackground(true);
    dl->setPalette(palette);
    dl->show();
    ui->horizontalLayout->addWidget(dl);
}
