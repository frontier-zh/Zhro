#include "unicodetozh.h"
#include "ui_unicodetozh.h"
#include "public_define.h"

UnicodeTozh::UnicodeTozh(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UnicodeTozh)
{
    ui->setupUi(this);
    this->ui->pushButton->setToolTip("<font color='#ff0000'>请确定转换内容中含有 类似字符\\u9752\\u5C9B </font>");
    this->ui->plainTextEdit->setToolTip("输入内容中含有类似字符\\u9752\\u5C9B");
}

UnicodeTozh::~UnicodeTozh()
{
    delete ui;
}

void UnicodeTozh::on_pushButton_clicked()
{
    QString  src= this->ui->plainTextEdit->toPlainText();
    QLOG_TRACE() << "ifnot include \\u : " << src.contains("\\u");
    if( src.contains("\\u") ){
        QString  des= fromEncodedUnicode(src);
        this->ui->plainTextEdit_2->setPlainText(des);
    }
}

void UnicodeTozh::on_pushButton_2_clicked()
{
    QString  src= this->ui->plainTextEdit_2->toPlainText();
    if( !src.isEmpty() ){
        QString  des= toEncodedUnicode(src);
        this->ui->plainTextEdit->setPlainText(des);
    }
}

QString
UnicodeTozh::fromEncodedUnicode(QString in)
{
        QString out;
        QChar aChar;
        int off = 0;
        int len = in.size();
        while (off < len) {
            aChar = in.at(off++);
            if (aChar == '\\') {
                if( off == len ){
                    break;
                }
                aChar = in.at(off++);
                if (aChar == 'u') {
                    // Read the xxxx
                    if( off + 3 > len - 1 ){
                        break;
                    }
                    int value = 0;
                    for (int i = 0; i < 4; i++) {
                        aChar = in.at(off++);
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
        return  out;
}

QString UnicodeTozh::toEncodedUnicode(QString in)
{
    QString out;
    for( int i= 0; i < in.size(); i++ ){
        QChar aChar = in.at(i);
        if ((aChar > 61) && (aChar < 127)) {
            if (aChar == '\\') {
                out.append('\\');
                out.append('\\');
                continue;
            }
            out.append(aChar);
            continue;
        }
        if( aChar == '\t' || aChar == '\n' || aChar == '\r' || aChar == '\f' ){
            if( aChar == '\t' ){
                out.append('\\');
                out.append('t');
            }else if( aChar == '\n' ){
                out.append('\\');
                out.append('n');
            }else if( aChar == '\r' ){
                out.append('\\');
                out.append('r');
            }else if( aChar == '\f' ){
                out.append('\\');
                out.append('f');
            }
        }
        if ((aChar < 0x0020) || (aChar > 0x007e)) {
            // 每个unicode有16位，每四位对应的16进制从高位保存到低位
            out.append('\\');
            out.append('u');
            ushort v = (aChar.unicode() >> 12) & 0xF;
            if( v < 10){
                out.append( v + '0' );
            }else{
                out.append( v - 10 + 'a');
            }
            v = (aChar.unicode() >> 8) & 0xF;
            if( v < 10){
                out.append( v + '0' );
            }else{
                out.append( v - 10 + 'a');
            }
            v = (aChar.unicode() >> 4) & 0xF;
            if( v < 10){
                out.append( v + '0' );
            }else{
                out.append( v - 10 + 'a');
            }
            v =  aChar.unicode() & 0xF;
            if( v < 10){
                out.append( v + '0' );
            }else{
                out.append( v - 10 + 'a');
            }
        } else {
            out.append(aChar);
        }

        }
        return out;
}
