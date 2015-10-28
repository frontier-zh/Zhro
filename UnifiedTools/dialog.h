#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QSslConfiguration>
#include <QtCore>

#define SAFE_DELETE(p)  if(p){delete p; p=NULL;}
typedef  QPair<QByteArray,QByteArray>   RHQPair;

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    QNetworkAccessManager *manager;
    QNetworkRequest *request;
    QNetworkReply   *reply;
    QNetworkCookieJar   *cookieJar;
    QSslConfiguration   config;

    void    urlEncodeChinese(QString &url, QString &encode);

private slots:

    void    on_pushButton_clicked();

    void    httpRespone();
    void    reqNonRespone();
    void    on_comboBox_2_currentTextChanged(const QString &arg1);

    void    confirmErrortype(QNetworkReply::NetworkError, QString &);

    void    httpError(QNetworkReply::NetworkError errcode);

private:
    Ui::Dialog *ui;
    QTimer              *_timer;
    QList<QNetworkCookie>   clist;
};

#endif // DIALOG_H
