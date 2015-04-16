#ifndef UNICODETOZH_H
#define UNICODETOZH_H

#include <QDialog>
#include <QtCore>

namespace Ui {
class UnicodeTozh;
}

class UnicodeTozh : public QDialog
{
    Q_OBJECT

public:
    explicit UnicodeTozh(QWidget *parent = 0);
    ~UnicodeTozh();


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::UnicodeTozh *ui;
    QString fromEncodedUnicode(QString in);
    QString toEncodedUnicode(QString in);
};

#endif // UNICODETOZH_H
