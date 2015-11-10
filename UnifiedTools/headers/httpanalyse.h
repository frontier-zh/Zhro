#ifndef HTTPANALYSE_H
#define HTTPANALYSE_H

#include <QDialog>
#include <QtCore>

namespace Ui {
class HttpAnalyse;
}

class HttpAnalyse : public QDialog
{
    Q_OBJECT

public:
    explicit HttpAnalyse(QWidget *parent = 0);
    ~HttpAnalyse();
    void    clearCacheRemain();

private slots:
    void on_button_clicked();
    void loadthreadFinished();

private:
    Ui::HttpAnalyse *ui;
};

#endif // HTTPANALYSE_H
