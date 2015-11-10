#include "taskimport.h"
#include "ui_taskimport.h"
#include "datahandle.h"
#include "taskimportthread.h"

TaskImport::TaskImport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TaskImport), idx(0), isover(true)
{
    qRegisterMetaType< DBINFO >("DBINFO");
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(addPropertyTowork()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(delPropertyTowork()));
    connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(dataProcessTowork()));
    ui->ddbport->setPlaceholderText("3306");
    ui->sdbport->setPlaceholderText("1433");
    ui->sdbhost->insertItems(0, DataHandle::instance().getHostList());
    ui->sdbname->insertItems(0, DataHandle::instance().getExpDbList());
    ui->sdbuser->insertItems(0, DataHandle::instance().getExpUserList());
    ui->sdbpwd->insertItems( 0, DataHandle::instance().getExpPassList());
    ui->ddbhost->insertItems(0, DataHandle::instance().getImpHostList());
    ui->ddbname->insertItems(0, DataHandle::instance().getImpDbList());
    ui->ddbuser->insertItems(0, DataHandle::instance().getImpUserList());
    ui->ddbpwd->insertItems( 0, DataHandle::instance().getImpPassList());

    QStringList  titles;
    QString  title = ui->label->text();
    QString  title5 = ui->label_5->text();
    QString  title2 = ui->label_2->text();
    QString  title3 = ui->label_3->text();
    QString  title4 = ui->label_4->text();
    titles << title << title5 << title2 << title3 << title4 << "导入条件";
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setColumnWidth(0,140);
    ui->tableWidget->setColumnWidth(1,60);
    ui->tableWidget->setColumnWidth(2,90);
    ui->tableWidget->setColumnWidth(3,80);
    ui->tableWidget->setColumnWidth(4,90);
    ui->tableWidget->setColumnWidth(5,380);
    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->setHorizontalHeaderLabels(titles);
}

TaskImport::~TaskImport()
{
    delete ui;
}

void TaskImport::addPropertyTowork()
{
    int  rows = ui->tableWidget->rowCount();
    QString col = ui->sdbhost->currentText();
    for( int i=0; i < rows; i++ ){
        QTableWidgetItem *ite = ui->tableWidget->item(i,0);
        if( !ite->text().compare(col) ){
            QMessageBox::warning(this,"attention","不能选取使用同一IP! 请更换源导入地址.");
            return;
        }
    }
    idx = rows;
    ui->tableWidget->setRowCount(idx+1);

    QString col1 = ui->sdbport->text();
    if( col1.isEmpty() ){
        col1 = ui->sdbport->placeholderText();
    }
    QString col2 = ui->sdbname->currentText();
    QString col3 = ui->sdbuser->currentText();
    QString col4 = ui->sdbpwd->currentText();

    QTableWidgetItem *item = new QTableWidgetItem(col);
    QTableWidgetItem *item1 = new QTableWidgetItem(col1);
    QTableWidgetItem *item2 = new QTableWidgetItem(col2);
    QTableWidgetItem *item3 = new QTableWidgetItem(col3);
    QTableWidgetItem *item4 = new QTableWidgetItem(col4);
    QTableWidgetItem *item5 = new QTableWidgetItem("");
    item->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    item1->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    item2->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    item3->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    item4->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    item5->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);

    ui->tableWidget->setItem(idx, 0, item);
    ui->tableWidget->setItem(idx, 1, item1);
    ui->tableWidget->setItem(idx, 2, item2);
    ui->tableWidget->setItem(idx, 3, item3);
    ui->tableWidget->setItem(idx, 4, item4);
    ui->tableWidget->setItem(idx, 5, item4);
    ui->tableWidget->resizeRowsToContents();
    idx++;
}

void TaskImport::delPropertyTowork()
{
    int row = ui->tableWidget->currentRow();
    ui->tableWidget->removeRow(row);
}

void TaskImport::dataProcessTowork()
{
    this->isover = true;
    ui->pushButton_3->setVisible(false);
    QList<DBINFO> infos;
    DBINFO info;
    info.host = ui->ddbhost->currentText();
    info.port = ui->ddbport->text().isEmpty() ? ui->ddbport->placeholderText().toInt() : ui->ddbport->text().toInt();
    info.dbname = ui->ddbname->currentText();
    info.tasktablename = "task";
    info.patterntablename = "task_fields";
    info.uid = ui->ddbuser->currentText();
    info.pwd = ui->ddbpwd->currentText();
    info.condstring = "";
//    qDebug() << info.host << " | " << info.port << " | " << info.dbname << " | " << info.tasktablename
//             << " | " << info.patterntablename << " | " << info.uid << " | " << info.pwd << " | " << info.condstring;
    infos.append(info);

    int     rows = ui->tableWidget->rowCount();
    for(int i=0; i<rows; i++){
        DBINFO info;
        QTableWidgetItem *ite = ui->tableWidget->item(i,0);
        info.host = ite->text();
        ite = ui->tableWidget->item(i,1);
        info.port = ite->text().toInt();
        ite = ui->tableWidget->item(i,2);
        info.dbname = ite->text();
        ite = ui->tableWidget->item(i,3);
        info.uid = ite->text();
        ite = ui->tableWidget->item(i,4);
        info.pwd = ite->text();
        ite = ui->tableWidget->item(i,5);
        if( ite ){
            info.condstring = ite->text();
        }else{
            info.condstring = "1=1";
        }
        //info.condstring = ite->text().isEmpty() ? "1=1" : ite->text();
        info.tasktablename = "Task";
        info.patterntablename = "TaskFields";

        infos.append(info);
    }

    TaskImportThread *thread = new TaskImportThread(this);
    thread->setHostList(infos);
    connect(thread, SIGNAL(finished()), this, SLOT(loadthreadFinished()));
    connect(thread, SIGNAL(notifyConsole(DBINFO)), this, SLOT(notethreadNotified(DBINFO)));
    thread->start();

}

void TaskImport::notethreadNotified(DBINFO info)
{
    isover = false;
    ui->pushButton_3->setVisible(true);
    QMessageBox::warning(this,"attention",QString("数据库连接失败[%1][%2][%3][%4]")
                         .arg(info.host).arg(info.dbname).arg(info.uid).arg(info.pwd));
}

void TaskImport::loadthreadFinished()
{
    if( isover ){
        ui->pushButton_3->setVisible(true);
        QMessageBox::information(this,"information","旧采集系统任务导入已完成!");
    }
}
