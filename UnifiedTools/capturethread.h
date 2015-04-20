#ifndef CAPTURETHREAD_H
#define CAPTURETHREAD_H

#include <QThread>
#include <QtCore>

class CaptureThread : public QThread
{
    Q_OBJECT
public:
    explicit CaptureThread(QObject *parent = 0);
    void    setUrlString(const QString &urlstring){
        this->url = urlstring;
    }
    QMap<QString, QVariant> getData(){
        return  result;
    }

protected:
    void    run();
    void    obtain(QString str);

signals:

public slots:

private:

    QString     url;
    QMap<QString, QVariant>  result;
    bool        isfinish;
    bool        ispacket;
    bool        ismethod;

};

#endif // CAPTURETHREAD_H
