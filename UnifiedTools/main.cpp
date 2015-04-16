#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
//    w.setAutoFillBackground(true);
//    QPalette palette;
//    QPixmap pixmap(":/image/4.jpg");
//    palette.setBrush(QPalette::Window, QBrush(pixmap));
//    w.setPalette(palette);
    w.show();

    return a.exec();
}
