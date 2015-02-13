#include "dialog.h"
#include "ui_dialog.h"
#include <QTextCodec>
#include <QNetworkCookie>
#include <QNetworkCookieJar>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{

    manager = new QNetworkAccessManager();
    request = new QNetworkRequest();
    reply   = NULL;
    QStringList  encode,method;
    encode << "GB2312" << "UTF-8" << "Big5";
    method << "get" << "post";
    ui->setupUi(this);
    this->ui->comboBox->insertItems(0,encode);
    this->ui->comboBox_2->insertItems(0,method);
}

Dialog::~Dialog()
{
    delete ui;
    reply->deleteLater();
    reply = NULL;
    delete request;
    delete manager;
}

void
Dialog::on_pushButton_clicked()
{
    QString address = this->ui->httpreq->toPlainText();
    QString postinf = this->ui->poststr->toPlainText();
    QString scookie = this->ui->cookiestring->toPlainText();
    QVariant refurl = this->ui->referurl->toPlainText();
    QNetworkCookieJar       cookieJar;
    QList<QNetworkCookie>   cookieList;
    QUrl url(address);
    request->setUrl(url);
    request->setHeader(QNetworkRequest::UserAgentHeader,"Mozilla/5.0 (Windows NT 6.3; Trident/7.0; rv:11.0) like Gecko");
    request->setRawHeader("Content-Type","application/x-www-form-urlencoded");
    //request->setHeader(QNetworkRequest::UserAgentHeader,"Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/35.0.1916.114 Safari/537.36");
    if( !scookie.isEmpty()){

        qDebug() << "jdkajkdjk";
        foreach(QString item,  scookie.split(";")){
            QVariant key = item.split("=").at(0);
            QVariant value = item.split("=").at(1);
            cookieList.append(QNetworkCookie(key.toByteArray(),value.toByteArray()));
        }
        if(cookieList.size()){
            cookieJar.setCookiesFromUrl(cookieList, url);
        }
    }
    if( !refurl.toString().isEmpty()){
        request->setRawHeader("Referer",refurl.toByteArray());
    }

    if( !postinf.isEmpty() ){
        QByteArray      post_data;
        post_data.append(postinf);
        request->setRawHeader("Content-Length",QByteArray::number(post_data.size()));
        reply = manager->post(*request, post_data);
    }else{
        reply = manager->get(*request);
    }
    connect(reply,SIGNAL(finished()),this,SLOT(httpRespone()));
    this->ui->pushButton->hide();
}

void
Dialog::httpRespone()
{
    QString  errtype = "";
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    this->ui->httprsp->setText(status_code.toString());
    foreach( RHQPair item, reply->rawHeaderPairs()){
        if( item.first.contains("Location") ){
            this->ui->httpgoto->setPlainText(item.second.data());
            break;
        }
        this->ui->httpgoto->setPlainText("");
    }
    this->ui->pushButton->show();
    confirmErrortype(reply->error(),errtype);
    this->ui->httperrcode->setText(errtype);
    this->ui->httperrmsg->setPlainText(reply->errorString());

    QByteArray web = reply->readAll();
    QString encode = this->ui->comboBox->currentText();  
    QString webcontext = QTextCodec::codecForName(encode.toUtf8())->toUnicode(web);
    this->ui->httpcontent->setPlainText(webcontext);
}

void
Dialog::on_comboBox_2_currentTextChanged(const QString &meth)
{
    if(meth.contains("get")){
        this->ui->poststr->hide();
        this->ui->poststr->setPlainText("");
    }else{
        this->ui->poststr->show();
        this->ui->poststr->setPlainText("");
    }
}

void Dialog::confirmErrortype(QNetworkReply::NetworkError err, QString &typ)
{
    switch (err) {
        case QNetworkReply::NoError:
            typ = " 0 NoError";
            break;
        case QNetworkReply::ConnectionRefusedError:
            typ = " 1 ConnectionRefusedError";
            break;
        case QNetworkReply::RemoteHostClosedError:
            typ = " 2 RemoteHostClosedError";
            break;
        case QNetworkReply::HostNotFoundError:
            typ = " 3 HostNotFoundError";
            break;
        case QNetworkReply::TimeoutError:
            typ = " 4 TimeoutError";
            break;
        case QNetworkReply::OperationCanceledError:
            typ = " 5 OperationCanceledError";
            break;
        case QNetworkReply::SslHandshakeFailedError:
            typ = " 6 SslHandshakeFailedError";
            break;
        case QNetworkReply::TemporaryNetworkFailureError:
            typ = " 7 TemporaryNetworkFailureError";
            break;
        case QNetworkReply::NetworkSessionFailedError:
            typ = " 8 NetworkSessionFailedError";
            break;
        case QNetworkReply::BackgroundRequestNotAllowedError:
            typ = " 9 BackgroundRequestNotAllowedError";
            break;
        case QNetworkReply::UnknownNetworkError:
            typ = " 99 UnknownNetworkError";
            break;
        case QNetworkReply::ProxyConnectionRefusedError:
            typ = " 101 ProxyConnectionRefusedError";
            break;
        case QNetworkReply::ProxyConnectionClosedError:
            typ = " 102 ProxyConnectionClosedError";
            break;
        case QNetworkReply::ProxyNotFoundError:
            typ = " 103 ProxyNotFoundError";
            break;
        case QNetworkReply::ProxyTimeoutError:
            typ = " 104 ProxyTimeoutError";
            break;
        case QNetworkReply::ProxyAuthenticationRequiredError:
            typ = " 105 ProxyAuthenticationRequiredError";
            break;
        case QNetworkReply::UnknownProxyError:
            typ = " 199 UnknownProxyError";
            break;
        case QNetworkReply::ContentAccessDenied:
            typ = " 201 ContentAccessDenied";
            break;
        case QNetworkReply::ContentOperationNotPermittedError:
            typ = " 202 ContentOperationNotPermittedError";
            break;
        case QNetworkReply::ContentNotFoundError:
            typ = " 203 ContentNotFoundError";
            break;
        case QNetworkReply::AuthenticationRequiredError:
            typ = " 204 AuthenticationRequiredError";
            break;
        case QNetworkReply::ContentReSendError:
            typ = " 205 ContentReSendError";
            break;
        case QNetworkReply::UnknownContentError:
            typ = " 299 UnknownContentError";
            break;
        case QNetworkReply::ProtocolUnknownError:
            typ = " 301 ProtocolUnknownError";
            break;
        case QNetworkReply::ProtocolInvalidOperationError:
            typ = " 302 ProtocolInvalidOperationError";
            break;
        case QNetworkReply::ProtocolFailure:
            typ = " 399 ProtocolFailure";
            break;
        default:
            break;
    }
}
