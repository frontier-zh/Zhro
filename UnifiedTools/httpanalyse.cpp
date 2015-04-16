#include "httpanalyse.h"
#include "ui_httpanalyse.h"
#include "capturethread.h"


HttpAnalyse::HttpAnalyse(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HttpAnalyse)
{
    ui->setupUi(this);
}

HttpAnalyse::~HttpAnalyse()
{
    delete ui;
}

void
HttpAnalyse::on_button_clicked()
{
    this->ui->button->setVisible(false);
    QString url = this->ui->edit->text();
    if( !url.isEmpty() ){
        if( url.contains("http://") ){
            url = url.mid(url.indexOf("/",10));
        }
        CaptureThread *thread = new CaptureThread(this);
        thread->setUrlString(url);
        connect(thread, SIGNAL(finished()), this, SLOT(loadthreadFinished()));
        thread->start();
    }
}

void
HttpAnalyse::loadthreadFinished()
{
    this->ui->button->setVisible(true);
    CaptureThread *thread = qobject_cast<CaptureThread*>(sender());
    QMap<QString,QVariant> info = thread->getData();
    foreach(QString key, info.keys()){
        qDebug() << "key: " << info.value(key);
        if( key.contains("Accept:", Qt::CaseInsensitive)){
            this->ui->edit1->setText(info.value(key).toString());
        }else if( key.contains("Content-Type:",Qt::CaseInsensitive)){
            this->ui->edit2->setText(info.value(key).toString());
        }else if( key.contains("Referer:",Qt::CaseInsensitive)){
            this->ui->edit3->setText(info.value(key).toString());
        }else if( key.contains("Accept-Language:",Qt::CaseInsensitive)){
            this->ui->edit4->setText(info.value(key).toString());
        }else if( key.contains("Accept-Encoding:",Qt::CaseInsensitive)){
            this->ui->edit5->setText(info.value(key).toString());
        }else if( key.contains("User-Agent:",Qt::CaseInsensitive)){
            this->ui->edit6->setText(info.value(key).toString());
        }else if( key.contains("Cookie:",Qt::CaseInsensitive)){
            this->ui->plainedit->setPlainText(info.value(key).toString());
        }else if( key.contains("Post:",Qt::CaseInsensitive)){
            this->ui->plainedit1->setPlainText(info.value(key).toString());
        }
    }
}
