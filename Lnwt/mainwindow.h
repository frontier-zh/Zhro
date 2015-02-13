#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void    setRelateproperty(QString);
    void    addPropertyTowork();
    void    delPropertyTowork();
    void    dataProcessTowork();
private:
    Ui::MainWindow *ui;
    int     idx;
};

#endif // MAINWINDOW_H
