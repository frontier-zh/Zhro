#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkCookie>
#include <QNetworkCookieJar>

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

private slots:

    void    on_pushButton_clicked();

    void    httpRespone();

    void    on_comboBox_2_currentTextChanged(const QString &arg1);

    void    confirmErrortype(QNetworkReply::NetworkError, QString &);

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
