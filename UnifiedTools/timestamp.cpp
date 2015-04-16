#include "timestamp.h"
#include "ui_timestamp.h"

TimeStamp::TimeStamp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TimeStamp)
{
    ui->setupUi(this);
    QStringList list;
    list << "yyyy-MM-dd hh:mm:ss:zzz"
         << "yyyyMMddhhmmsszzz"
         << "yyyy MM dd hh:mm:ss:zzz"
         << "yyyy/MM/dd hh:mm:ss:zzz"
         << "MM/dd/yyyy hh:mm:ss:zzz"
         << "yyyy年MM月dd日 hh:mm:ss:zzz"
         << "yyyy年MM月dd日 hh时mm分ss秒"
         << "yy年MM月dd日 hh:mm:ss:zzz"
         << "MM dd yyyy hh:mm:ss:zzz";
    this->ui->comboBox->insertItems(0,list);
    QRegExp regxp("[0-9]{10}[.]{0,1}[0-9]{0,3}");
    this->ui->lineEdit->setValidator(new QRegExpValidator(regxp,this)); //设置编辑框只能输入特定格式.
    this->ui->lineEdit_2->setEnabled(false);
    this->ui->lineEdit_4->setEnabled(false);    //设置不能编辑模式,只做显示.
}

TimeStamp::~TimeStamp()
{
    delete ui;
}

void TimeStamp::on_button_clicked()
{
    QString  stamp = this->ui->lineEdit->text();
    QString  dtime = this->ui->lineEdit_3->text();
    QString  format = this->ui->comboBox->currentText();
    QString  result;
    qint64   itamp = 0;
    if( !stamp.isEmpty() ){
        if( stamp.size() == 10 ){
            itamp = stamp.toLongLong();
            itamp = itamp * 1000;
            result = QDateTime::fromMSecsSinceEpoch(itamp).toString(format);
        }else if( stamp.size() == 13 && !stamp.contains(".") ){
            itamp = stamp.toLongLong();
            result = QDateTime::fromMSecsSinceEpoch(itamp).toString(format);
        }else if( stamp.size() == 14 &&  stamp.contains(".") ){
            stamp.replace(".", "");
            itamp = stamp.toLongLong();
            result = QDateTime::fromMSecsSinceEpoch(itamp).toString(format);
        }
        this->ui->lineEdit_2->setText(result);
    }
    bool isls = false;
    if( !dtime.isEmpty() ){
        qDebug() << format;
        qDebug() << dtime;
        if( dtime.contains(":") ){
            QStringList etime = dtime.split(":");
            QString     slast = etime.at(etime.size()-1);
            if( slast.size() != 3 ){
                format.replace(":zzz", "");
                isls = true;
            }
        }
        qDebug() << format;
        QDateTime  now = QDateTime::fromString(dtime, format);
        qDebug() << now.toMSecsSinceEpoch();
        result = QString::number(now.toMSecsSinceEpoch());
        qDebug() << result;
        if( isls ){
            result = result.mid(0, result.size() - 3 );
        }
        this->ui->lineEdit_4->setText(result);
    }
}
