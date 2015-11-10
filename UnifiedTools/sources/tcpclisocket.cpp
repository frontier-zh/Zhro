#include "tcpclisocket.h"
#include "ui_tcpclisocket.h"
#include <QMessageBox>
#include <QHostAddress>
#include <QDebug>

TcpCliSocket::TcpCliSocket(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TcpCliSocket), IP(""), Port(0), packbytes(0),
    oncebytes(0), remainbytes(0), readbuff(""), writebuff("")
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()), SLOT(newConnection()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(onDisconnection()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    connect(socket,SIGNAL(bytesWritten(qint64)),this,SLOT(onBytesWritten(qint64)));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
                    SLOT(onErrorProcess(QAbstractSocket::SocketError)));

    ui->dis_button->setHidden(true);
    ui->snd_button->setHidden(true);

    connect(ui->con_button, SIGNAL(clicked()), SLOT(connectServer()));
    connect(ui->dis_button, SIGNAL(clicked()), SLOT(onDisconnection()));
    connect(ui->snd_button, SIGNAL(clicked()), SLOT(sndpackageServer()));

    ui->pkg_upload->setToolTip("<font color='#ff0000'>请输入内容要发送的报文.</font>");
    ui->ip_lineedit->setPlaceholderText("请输入远程IP地址.");

}

TcpCliSocket::~TcpCliSocket()
{
    if( socket->isOpen() ){
        onDisconnection();
    }
    delete  socket;
    socket = NULL;
    delete ui;
}

void
TcpCliSocket::connectServer()
{
    qDebug() << __FILE__ << ":" << __LINE__ << __FUNCTION__;
    IP = ui->ip_lineedit->text();
    Port = ui->port_lineedit->text().toInt();
    if( IP.isEmpty() || Port < 2000 ){
        QMessageBox::information(this, "当心","IP地址不能为空、Port端口大于2000");
        return;
    }

    socket->connectToHost(QHostAddress(IP),Port);

}

void
TcpCliSocket::sndpackageServer()
{
    ui->pkg_download->clear();
    ui->pkg_download->setPlainText("");

    qDebug() << __FILE__ << ":" << __LINE__ << __FUNCTION__;
    QByteArray  buff(ui->pkg_upload->toPlainText().toLatin1());
    if( buff.isNull() || buff.isEmpty() ){
        QMessageBox::information(this, "attention","请输入需要发送的报文");
        return;
    }

    if( socket == NULL ||
        socket->state() != QAbstractSocket::ConnectedState ){
        QMessageBox::information(this, "attention","通信连接丢失请重新连接");
        return;
    }

    if( writebuff.size() ==0 || writebuff.isEmpty()){
        packbytes = buff.size();
        oncebytes = socket->write(buff);
        if( oncebytes == -1 ){
            writebuff.append(buff);
            return;
        } else {
            if( oncebytes < packbytes ){
                writebuff = buff.mid(oncebytes);
            } else {
                writebuff.clear();
            }
        }
    } else {
        writebuff.append(buff);
    }
}

void
TcpCliSocket::newConnection()
{
    qDebug() << __FILE__ << ":" << __LINE__ << __FUNCTION__;
    qDebug() << socket->peerName() << "|" << socket->peerAddress() << ":" << socket->peerPort();
    ui->dis_button->setHidden(false);
    ui->snd_button->setHidden(false);
    QMessageBox::information(this, "提示","通信连接OK:请在报文区放入要发送文本");
}

void
TcpCliSocket::onDisconnection()
{
    qDebug() << __FILE__ << ":" << __LINE__ << __FUNCTION__;
    if( socket->isOpen() ){
        socket->close();
    }
    ui->dis_button->setHidden(true);
    ui->snd_button->setHidden(true);
}

void
TcpCliSocket::onReadyRead()
{
    qDebug() << __FILE__ << ":" << __LINE__ << __FUNCTION__;

    if( socket == NULL ||
        socket->state() != QAbstractSocket::ConnectedState ){
        return;
    }
    while( socket->bytesAvailable() > 0 ){
        QByteArray  buff = socket->readAll();
        ui->pkg_download->insertPlainText(buff);
    }
}

void
TcpCliSocket::onBytesWritten(qint64)
{
    qDebug() << __FILE__ << ":" << __LINE__ << __FUNCTION__;
    if( writebuff.size() == 0 || writebuff.isEmpty() ){
        return;
    }

    remainbytes = writebuff.size();
    oncebytes = socket->write(writebuff);
    if( oncebytes == -1 ){
        return;
    } else {
        if( oncebytes < remainbytes ){
            writebuff = writebuff.mid(oncebytes);
        } else {
            writebuff.clear();
        }
    }
}

void
TcpCliSocket::onErrorProcess(QAbstractSocket::SocketError)
{
    qDebug() << __FILE__ << ":" << __LINE__ << __FUNCTION__;
}
