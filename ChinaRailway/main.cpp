#include "mainwidget.h"
#include <QApplication>

const char* FONT_NAME = "Hiragino Sans GB W6";
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget w;
    w.resize(960,540);
    w.show();

    return a.exec();
}
