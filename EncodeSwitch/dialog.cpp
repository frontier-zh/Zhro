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
    this->ui->lineEdit_2->setText( url );
    //QString  txt= QString(this->ui->lineEdit_3->text().constData());

    QString  src= this->ui->plainTextEdit->toPlainText();
    QString  des= fromEncodedUnicode(src);
    this->ui->plainTextEdit_2->setPlainText(des);

//    foreach(QString item, src.split("\\u")){
//        qDebug() << item << " " << item.toInt(0,16);
//        char  chb= ((char)item.toInt(0,16));
//        qDebug() << chb;
//        QByteArray bar = QByteArray(1,chb);
//        qDebug() << QTextCodec::codecForName("GBK")->toUnicode(bar);
//    }



//    QString  text= "\u623F\u578B";
//    qDebug() << text << " "  << text.size();
//    this->ui->lineEdit_4->setText( text );
//    char*  ch1= "\u623f";
//    char*  ch2= "\u578B";
//    QString  txt1 = ch1;
//    txt1.append(ch2);
//    qDebug() << txt1;

}


QString
Dialog::fromEncodedUnicode(QString in)
{
        QString out;
        QChar aChar;
        int off = 0;
        int len = in.size();
        while (off < len) {
            aChar = in.at(off++);
            qDebug() << aChar;
            if (aChar == '\\') {
                aChar = in.at(off++);
                qDebug() << aChar;
                if (aChar == 'u') {
                    qDebug() << aChar;
                    // Read the xxxx
                    int value = 0;
                    for (int i = 0; i < 4; i++) {
                        aChar = in.at(off++);
                        qDebug() << aChar << " " << aChar.toLatin1();
                        switch (aChar.toLatin1()) {
                        case '0':
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7':
                        case '8':
                        case '9':
                            value = (value << 4) + aChar.toLatin1() - '0';
                            break;
                        case 'a':
                        case 'b':
                        case 'c':
                        case 'd':
                        case 'e':
                        case 'f':
                            value = (value << 4) + 10 + aChar.toLatin1() - 'a';
                            break;
                        case 'A':
                        case 'B':
                        case 'C':
                        case 'D':
                        case 'E':
                        case 'F':
                            value = (value << 4) + 10 + aChar.toLatin1() - 'A';
                            break;
                        default:
                            break;
                        }
                    }
                    out.append(QChar(value));
                    qDebug() << value << " " << QChar(value);

                } else {
                    if (aChar == 't') {
                        aChar = '\t';
                    } else if (aChar == 'r') {
                        aChar = '\r';
                    } else if (aChar == 'n') {
                        aChar = '\n';
                    } else if (aChar == 'f') {
                        aChar = '\f';
                    }
                    out.append(aChar);
                }
            } else {
                out.append(aChar);
            }
        }
        qDebug() << out;
        qDebug() << "-------------------------";
        return  out;
}
