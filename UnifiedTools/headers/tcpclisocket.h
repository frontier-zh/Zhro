#ifndef TCPCLISOCKET_H
#define TCPCLISOCKET_H

#include <QDialog>
#include <QTcpSocket>

namespace Ui {
class TcpCliSocket;
}

class TcpCliSocket : public QDialog
{
    Q_OBJECT

public:
    explicit TcpCliSocket(QWidget *parent = 0);
    ~TcpCliSocket();

public slots:

    void    connectServer();
    void    sndpackageServer();

    void    newConnection();
    void    onDisconnection();
    void    onReadyRead();
    void    onBytesWritten(qint64);
    void    onErrorProcess(QAbstractSocket::SocketError);

private:
    Ui::TcpCliSocket *ui;

    QTcpSocket  *socket;

    QString     IP;
    int         Port;

    qint64      packbytes;
    qint64      oncebytes;
    qint64      remainbytes;
    QByteArray  readbuff;
    QByteArray  writebuff;
};

#endif // TCPCLISOCKET_H
