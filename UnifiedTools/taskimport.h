#ifndef TASKIMPORT_H
#define TASKIMPORT_H

#include <QDialog>
#include <QtCore>
#include "public_define.h"

namespace Ui {
class TaskImport;
}

class TaskImport : public QDialog
{
    Q_OBJECT

public:
    explicit TaskImport(QWidget *parent = 0);
    ~TaskImport();

private slots:
    void    addPropertyTowork();
    void    delPropertyTowork();
    void    dataProcessTowork();
    void    notethreadNotified(DBINFO info);
    void    loadthreadFinished();
private:
    Ui::TaskImport *ui;
    int             idx;
    bool            isover;
};

#endif // TASKIMPORT_H
