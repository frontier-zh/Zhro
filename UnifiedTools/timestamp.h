#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <QDialog>
#include <QtCore>

namespace Ui {
class TimeStamp;
}

class TimeStamp : public QDialog
{
    Q_OBJECT

public:
    explicit TimeStamp(QWidget *parent = 0);
    ~TimeStamp();

private slots:
    void on_button_clicked();

private:
    Ui::TimeStamp *ui;
};

#endif // TIMESTAMP_H
