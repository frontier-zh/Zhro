#include "mainwindow.h"
#include <QApplication>
#include "public_define.h"
#include <QtCore>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    using namespace QsLogging;

    Logger& logger = Logger::instance();
    logger.setLoggingLevel(QsLogging::TraceLevel);
    const QString sLogPath(QDir(a.applicationDirPath()).filePath("log_tools.txt"));

    FileDestination  *fileDestination = new FileDestination(sLogPath, 10*1024*1024, 1 );
    logger.addDestination(fileDestination);
    MainWindow w;
//    w.setAutoFillBackground(true);
//    QPalette palette;
//    QPixmap pixmap(":/image/4.jpg");
//    palette.setBrush(QPalette::Window, QBrush(pixmap));
//    w.setPalette(palette);
    w.show();

    return a.exec();
}
