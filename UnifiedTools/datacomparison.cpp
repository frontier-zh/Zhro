#include "datacomparison.h"
#include "public_define.h"
#include "datahandle.h"
#include "ui_datacomparison.h"
#include "datacomparisonthread.h"

DataComparison::DataComparison(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataComparison), idx(0), previous(0), isload(true)
{
    qRegisterMetaType< QList<QStringList> >("QList<QStringList>");
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(comparisonTowork()));
    //ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QScrollBar *scrolbar = ui->tableWidget->verticalScrollBar();
    connect((QWidget*)scrolbar, SIGNAL(valueChanged(int)), this, SLOT(onSliderMovLoading(int)));
    ui->lineEdit->setPlaceholderText("请输入任务 datasource");
    //ui->lineEdit_2->setEnabled(false);
    ui->lineEdit_3->setEnabled(false);
    ui->ddbport->setPlaceholderText("1433");
    ui->sdbport->setPlaceholderText("1433");
    ui->sdbhost->insertItems(0, DataHandle::instance().getHostList());
    QStringList  dbname;
    dbname.append("data_get");
    dbname.append("data_gov");
    ui->sdbname->insertItems(0, dbname);
    ui->sdbuser->insertItem(0, "sa");
    ui->sdbpwd->insertItem(0, "2205886lyx");
    QStringList  dbhost;
    dbhost.append("10.10.65.192");
    ui->ddbhost->insertItems(0, dbhost);
    ui->ddbname->insertItem(0, "data_get");
    ui->ddbuser->insertItem(0, "sa");
    ui->ddbpwd->insertItem(0, "caiji.2014");

    /////
    QStringList  titles;
    titles.append("SourceUrl");
    titles.append("差异字段");
    titles.append("预处理");
    titles.append("[新]数据一");
    titles.append("[旧]数据二");
    titles.append("[新]获得时间");
    titles.append("[旧]获得时间");

    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setColumnWidth(0,300);
    ui->tableWidget->setColumnWidth(1,60);
    ui->tableWidget->setColumnWidth(2,50);
    ui->tableWidget->setColumnWidth(3,350);
    ui->tableWidget->setColumnWidth(4,350);
    ui->tableWidget->setColumnWidth(5,140);
    ui->tableWidget->setColumnWidth(6,140);
    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->setHorizontalHeaderLabels(titles);
}

DataComparison::~DataComparison()
{
    delete ui;
}

void DataComparison::comparisonTowork()
{
    idx = 0;
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(idx);
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    result.clear();
    previous = 0;
    isload = true;

    ui->pushButton->setVisible(false);
    QList<DBINFO> infos;
    DBINFO info;
    info.host = ui->sdbhost->currentText();
    info.port = ui->sdbport->text().isEmpty() ? ui->sdbport->placeholderText().toInt() : ui->sdbport->text().toInt();
    info.dbname = ui->sdbname->currentText();
    info.tasktablename = "dataget_origin";
    info.patterntablename = "";
    info.uid = ui->sdbuser->currentText();
    info.pwd = ui->sdbpwd->currentText();
    info.condstring = "";
    info.datasource = ui->lineEdit->text();
    if( info.datasource.isEmpty() ){
        QMessageBox::warning(this, "attention", "请输入需要对比的任务datasource!");
        ui->pushButton->setVisible(true);
        return;
    }
    QLOG_TRACE() << "datasource :   " << info.datasource;
    infos.append(info);

    info.host = ui->ddbhost->currentText();
    info.port = ui->ddbport->text().isEmpty() ? ui->ddbport->placeholderText().toInt() : ui->ddbport->text().toInt();
    info.dbname = ui->ddbname->currentText();
    info.tasktablename = "dataget_origin";
    info.patterntablename = "";
    info.uid = ui->ddbuser->currentText();
    info.pwd = ui->ddbpwd->currentText();
    info.condstring = "";
    info.datasource = ui->lineEdit->text();
    infos.append(info);


    DataComparisonThread *thread = new DataComparisonThread(this);
    thread->setHostList(infos);
    connect(thread, SIGNAL(finished()), this, SLOT(loadthreadFinished()));
    connect(thread, SIGNAL(stopFeekback(QString)), this, SLOT(stopthreadFeedback(QString)));
    thread->start();
}

void DataComparison::loadthreadFinished()
{
    ui->pushButton->setVisible(true);
    DataComparisonThread *thread = qobject_cast<DataComparisonThread*>(sender());
    result = thread->getResult();
    int pos = idx;
    int end = (pos + 150 < result.size()) ? pos + 150 : result.size();
    for(int i= pos; i < end; i++ ){
        QStringList data = result.at(i);
        int len = data.size();
        idx = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(idx+1);
        for( int i=0; i < len; i++ ){
            QTableWidgetItem *item = new QTableWidgetItem(data.at(i));
            item->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
            ui->tableWidget->setItem(idx, i, item);
        }
        ui->tableWidget->resizeRowsToContents();
        idx++;
    }

    QMessageBox::information(this, "information", "新、旧系统房源数据对比结束!");
    return;
}

void DataComparison::onSliderMovLoading(int position)
{

    if( previous == position ){
        int pos = idx;
        int end = (pos + 10 < result.size()) ? pos + 10 : result.size();
        for(int i= pos; i < end; i++ ){
            QStringList data = result.at(i);
            int len = data.size();
            idx = ui->tableWidget->rowCount();
            ui->tableWidget->setRowCount(idx+1);
            for( int i=0; i < len; i++ ){
                QTableWidgetItem *item = new QTableWidgetItem(data.at(i));
                item->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
                ui->tableWidget->setItem(idx, i, item);
            }
            ui->tableWidget->resizeRowsToContents();
            idx++;
        }
        previous = position + 10;
    }else{
        previous = position;
    }
}

void DataComparison::stopthreadFeedback(QString feedback)
{
    ui->lineEdit_2->setText(feedback);
}
