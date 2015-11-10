#include "httpanalyse.h"
#include "ui_httpanalyse.h"
#include "capturethread.h"
#include "public_define.h"


HttpAnalyse::HttpAnalyse(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HttpAnalyse)
{
    ui->setupUi(this);
    this->ui->edit->setPlaceholderText("请输入需要捕获的URL");
    this->ui->button->setToolTip("<font color='#ff0000'>请先输入需要捕获的Url <br>再'点击获取'并及时访问或刷新包含URL的页面.</font>");
}

HttpAnalyse::~HttpAnalyse()
{
    delete ui;
}

void HttpAnalyse::clearCacheRemain()
{
    this->ui->edit1->clear();
    this->ui->edit2->clear();
    this->ui->edit3->clear();
    this->ui->edit4->clear();
    this->ui->edit5->clear();
    this->ui->edit6->clear();
    this->ui->plainedit->clear();
    this->ui->plainedit1->clear();
}

void
HttpAnalyse::on_button_clicked()
{  
    QString url = this->ui->edit->text();
    if( !url.isEmpty() ){
        this->clearCacheRemain();
        this->ui->button->setVisible(false);
        if( url.contains("http://") ){
            url = url.mid(url.indexOf("/",10));
        }
        CaptureThread *thread = new CaptureThread(this);
        thread->setUrlString(url);
        connect(thread, SIGNAL(finished()), this, SLOT(loadthreadFinished()));
        thread->start();
    }else{
        QMessageBox::warning(this,"attention","设置抓包分析的URL不能为空!");
    }
}

void
HttpAnalyse::loadthreadFinished()
{
    this->ui->button->setVisible(true);
    CaptureThread *thread = qobject_cast<CaptureThread*>(sender());
    QMap<QString,QVariant> info = thread->getData();
    foreach(QString key, info.keys()){
        QLOG_TRACE() << key << " : " << info.value(key);
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
