#include "dialog.h"
#include "ui_dialog.h"
#include <QTextCodec>
#include "public_define.h"


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{

    manager = new QNetworkAccessManager();
    request = new QNetworkRequest();
    reply   = NULL;
    _timer  = new   QTimer();
    connect(_timer, SIGNAL(timeout()),  this, SLOT(reqNonRespone()));
    QStringList  encode,method;
    encode << "GB2312" << "UTF-8" << "Big5";
    method << "GET" << "POST";
    ui->setupUi(this);
    this->ui->comboBox->insertItems(0,encode);
    this->ui->comboBox_2->insertItems(0,method);
    this->ui->httpreq->setPlaceholderText("请输入访问测试的Url.");
    this->ui->pushButton->setToolTip("<font color='#ff0000'>点击访问前,请先选择获取方式和编码类型.</font>");
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
    clist.clear();
    QString address = this->ui->httpreq->text();
    QString postinf = this->ui->poststr->toPlainText();
    QString scookie = this->ui->cookiestring->toPlainText();
    QVariant refurl = this->ui->referurl->toPlainText();
    QString encode = this->ui->comboBox->currentText();

    urlEncodeChinese(address, encode);
    if( address.contains("%%") ){
        address.replace("%%", "%");
    }
    QUrl url(address);
    cookieJar = new QNetworkCookieJar();
    QList<QNetworkCookie>   cookieList;
    request->setUrl(url);
    request->setRawHeader("Content-Type","application/x-www-form-urlencoded");
    request->setHeader(QNetworkRequest::UserAgentHeader,"Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/35.0.1916.114 Safari/537.36");
    if( !scookie.isEmpty()){
        foreach(QString item,  scookie.split(";")){
            item = item.trimmed();
            QVariant key = item.split("=").at(0);
            QVariant value = item.mid(item.indexOf("=")+1);
            cookieList.append(QNetworkCookie(key.toByteArray(),value.toByteArray()));
        }
        if(cookieList.size()){
            cookieJar->setCookiesFromUrl(cookieList, url);
        }
        manager->setCookieJar(cookieJar);
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
    if( !_timer->isActive()){
        _timer->start(10000);
    }
    //connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(httpRespone()));
    connect(reply,SIGNAL(finished()),this,SLOT(httpRespone()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(httpError(QNetworkReply::NetworkError)));
    this->ui->pushButton->hide();
}

void
Dialog::httpRespone()
{
    if( _timer->isActive()){
        _timer->stop();
    }
    QString  errtype = "";
    QString  address = "";
    QString  requrl = reply->url().toString();
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    this->ui->httprsp->setText(status_code.toString());
    foreach( RHQPair item, reply->rawHeaderPairs()){
        qDebug() << item.first.data() << ":" << item.second.data();
        if((item.first.contains("Location") || item.first.contains("location"))
                && (status_code.toInt() == 301 || status_code.toInt() == 302) ){
            this->ui->httpgoto->setPlainText(item.second.data());
            address = QString(item.second.data());
            if( !address.contains("http://") ){
                if( address.indexOf("/") == 0 ){
                    int pos = requrl.indexOf("/",7);
                    if( pos != -1 ){
                        requrl = requrl.mid(0,requrl.indexOf("/",7));
                    }
                }else{
                    int pos = requrl.indexOf("/",7);
                    if( pos == -1 ){
                        requrl = requrl + "/";
                    }else{
                        requrl = requrl.mid(0,requrl.indexOf("/",7) + 1);
                    }
                }
                address = requrl + address;
            }
            break;
        }
        this->ui->httpgoto->setPlainText("");
    }
    QString cookies = "";
    foreach( RHQPair item, reply->rawHeaderPairs()){
        if( item.first.contains("Set-Cookie") ){
            cookies = QString(item.second.data());
            break;
        }
    }
    if( !cookies.isEmpty() ){
        foreach(QString part, cookies.split("\n")){
            bool    exist = true;
            QStringList subp = part.split(";");
            if( subp.size() ){
                qDebug() << "--------------------------";
                qDebug() << subp.at(0);
                QStringList text = subp.at(0).split("=");
                if( text.size() ){
                    QString key = text.at(0);
                    foreach(QNetworkCookie  item, clist){
                        if( !QString(item.name()).compare(key)){
                            exist = false;
                            break;
                        }
                    }
                    if( exist ){
                        QVariant key = text.at(0);
                        QVariant value = text.at(1);
                        qDebug() << "name: " << key << " value: " << value;
                        clist.append(QNetworkCookie(key.toByteArray(),value.toByteArray()));
                    }
                }
            }
        }
    }
    qDebug() << "cookie size:" << clist.size();
    if( !address.isEmpty() ){
        reply->deleteLater();
        reply = NULL;

        QUrl url(address);
        request->setUrl(url);
        request->setHeader(QNetworkRequest::UserAgentHeader,"Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/35.0.1916.114 Safari/537.36");
        request->setRawHeader("Accept-Language","zh-CN");
        request->setRawHeader("Referer","");
        if( clist.size() ){
            cookieJar->setCookiesFromUrl(clist, url);
        }
        manager->setCookieJar(cookieJar);
        reply = manager->get(*request);
        connect(reply,SIGNAL(finished()),this,SLOT(httpRespone()));
        return;
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
Dialog::reqNonRespone()
{
    if( _timer->isActive()){
        _timer->stop();
    }
    if( reply != NULL ){
        qDebug() << "request timeout: " << reply->url().toString() << reply->isOpen();
        if( reply->isOpen() ){
            qDebug() <<"respone data: " << reply->readAll();
            reply->close();
        }
    }
}

void
Dialog::on_comboBox_2_currentTextChanged(const QString &meth)
{
    if(meth.contains("GET")){
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

void
Dialog::httpError(QNetworkReply::NetworkError errcode)
{
    qDebug()<<"http request error: "<< errcode << reply;
}
void
Dialog::urlEncodeChinese(QString &url, QString &encode)
{
    int i = 0;
    while( i < url.size() )
    {
        QByteArray  ba,bt;
        QChar cha = url.at(i);
        ulong uni = cha.unicode();
        if(uni >= 0x4E00 && uni <= 0x9FA5)
        {
            QString  str = QString(url.at(i));
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
}
