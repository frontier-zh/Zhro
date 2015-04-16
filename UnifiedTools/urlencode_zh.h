#ifndef URLENCODE_ZH_H
#define URLENCODE_ZH_H

#include <QDialog>

namespace Ui {
    class UrlEncode_zh;
}

class UrlEncode_zh : public QDialog
{
    Q_OBJECT

public:
    explicit UrlEncode_zh(QWidget *parent = 0);
    ~UrlEncode_zh();

private:
    Ui::UrlEncode_zh *ui;
};

#endif // URLENCODE_ZH_H
