#include "urlencode_zh.h"
#include "ui_urlencode_zh.h"

UrlEncode_zh::UrlEncode_zh(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UrlEncode_zh)
{
    ui->setupUi(this);
}

UrlEncode_zh::~UrlEncode_zh()
{
    delete ui;
}
