#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <QDialog>
#include <QtCore>
#include <QTimer>

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
    void updateTimeStamp();

private:
    Ui::TimeStamp *ui;
    QTimer        *_time;
};

#endif // TIMESTAMP_H
