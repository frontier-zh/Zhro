#ifndef URLENCODE_H
#define URLENCODE_H

#include <QDialog>
#include <QtCore>

namespace Ui {
class UrlEncode;
}

class UrlEncode : public QDialog
{
    Q_OBJECT

public:
    explicit UrlEncode(QWidget *parent = 0);
    ~UrlEncode();

private slots:
    void on_pushButton_clicked();

    void on_button_clicked();

    void on_button_2_clicked();

private:
    Ui::UrlEncode *ui;
    QString     Simplified2Traditional(QString in);
    QString     Traditional2Simplified(QString in);
};

#endif // URLENCODE_H
