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
    ui->sdbhost->setPlaceholderText("10.10.65.194");
    ui->sdbname->setPlaceholderText("data_get");
    ui->sdbuser->setPlaceholderText("sa");
    ui->sdbpwd->setPlaceholderText("2205886lyx");
    ui->sdbtbl->setPlaceholderText("dataget_origin");
    ui->ddbhost->setPlaceholderText("10.10.65.192");
    ui->ddbname->setPlaceholderText("data_get");
    ui->ddbuser->setPlaceholderText("sa");
    ui->ddbpwd->setPlaceholderText("caiji.2014");
    ui->ddbtbl->setPlaceholderText("dataget_origin");

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
    info.host = ui->sdbhost->text().isEmpty() ? ui->sdbhost->placeholderText() : ui->sdbhost->text();
    info.port = ui->sdbport->text().isEmpty() ? ui->sdbport->placeholderText().toInt() : ui->sdbport->text().toInt();
    info.dbname = ui->sdbname->text().isEmpty() ? ui->sdbname->placeholderText() : ui->sdbname->text();
    info.tasktablename = ui->sdbtbl->text().isEmpty() ? ui->sdbtbl->placeholderText() : ui->sdbtbl->text();
    info.patterntablename = "";
    info.uid = ui->sdbuser->text().isEmpty() ? ui->sdbuser->placeholderText() : ui->sdbuser->text();
    info.pwd = ui->sdbpwd->text().isEmpty() ? ui->sdbpwd->placeholderText() : ui->sdbpwd->text();
    info.condstring = "";
    info.datasource = ui->lineEdit->text();
    if( info.datasource.isEmpty() ){
        QMessageBox::warning(this, "attention", "请输入需要对比的任务datasource!");
        ui->pushButton->setVisible(true);
        return;
    }
    QLOG_TRACE() << "datasource :   " << info.datasource;
    infos.append(info);

    info.host = ui->ddbhost->text().isEmpty() ? ui->ddbhost->placeholderText() : ui->ddbhost->text();
    info.port = ui->ddbport->text().isEmpty() ? ui->ddbport->placeholderText().toInt() : ui->ddbport->text().toInt();
    info.dbname = ui->ddbname->text().isEmpty() ? ui->ddbname->placeholderText() : ui->ddbname->text();
    info.tasktablename = ui->ddbtbl->text().isEmpty() ? ui->ddbtbl->placeholderText() : ui->ddbtbl->text();
    info.patterntablename = "";
    info.uid = ui->ddbuser->text().isEmpty() ? ui->ddbuser->placeholderText() : ui->ddbuser->text();
    info.pwd = ui->ddbpwd->text().isEmpty() ? ui->ddbpwd->placeholderText() : ui->ddbpwd->text();
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
