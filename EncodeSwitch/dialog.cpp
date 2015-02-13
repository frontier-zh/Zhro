#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QStringList list;
    list << "UTF-8" << "GB2312" << "GBK";
    this->ui->comboBox->insertItems(0,list);
}

Dialog::~Dialog()
{
    delete ui;
}

void
Dialog::on_pushButton_clicked()
{

    //    ba = QTextCodec::codecForName(encode.toUtf8())->fromUnicode(QTextCodec::codecForName("UTF-8")->toUnicode(chinese.toUtf8()));
    //    qDebug() << ba.toHex();
    //    qDebug() << ba.toHex().size();
    //    qDebug() << ba;
    //    this->ui->lineEdit_2->setText( ba.toHex());
    QByteArray  ba,ba3;
    QString  url= this->ui->lineEdit->text();
    //ba3 = QTextCodec::codecForName("UTF-8")->fromUnicode(url);     //Unicode QString to UTF-8 QByteArray

    QString  encode = this->ui->comboBox->currentText();
    int i = 0;
    while( i < url.size() )
    {
        QByteArray  bt;
        QChar cha = url.at(i);
        ulong uni = cha.unicode();
        if(uni >= 0x4E00 && uni <= 0x9FA5)
        {
            QString  str = QString(url.at(i));
            qDebug() << "中文：" << url.at(i);
            ba = QTextCodec::codecForName(encode.toUtf8())->fromUnicode(str);
            for(int j=0; j<ba.toHex().size(); j++)
            {
                if(j%2==0){
                    bt.append("%");
                }
                bt.append(ba.toHex().toUpper().at(j));
            }
            url.replace(url.at(i),bt);
        }
        i++;
    }
    this->ui->lineEdit_2->setText( url);

}

