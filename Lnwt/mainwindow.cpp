#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "datahandle.h"
#include <QTableWidget>
#include <QTableWidgetItem>
#include "cvs_fileparse.h"
#include "res_fileparse.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), idx(0)
{
    ui->setupUi(this);
    connect(ui->comboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(setRelateproperty(QString)));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(addPropertyTowork()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(delPropertyTowork()));
    connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(dataProcessTowork()));
    QStringList  filekind;
    filekind << "*.CSV" << "*.RES";
    QStringList  targetnum;
    targetnum << "汇总一个" << "各自生成";
    this->ui->comboBox->insertItems(0, DataHandle::instance().getProductList());
    this->ui->comboBox_3->insertItems(0, DataHandle::instance().getStandardList());
    this->ui->comboBox_4->insertItems(0, filekind);
    this->ui->comboBox_5->insertItems(0, targetnum);
    QStringList  titles;
    QString  title = this->ui->label->text();
    QString  title2 = this->ui->label_2->text();
    QString  title3 = this->ui->label_3->text();
    titles << title << title2 << title3 ;
    this->ui->tableWidget->setColumnCount(4);
    this->ui->tableWidget->setColumnWidth(0,180);
    this->ui->tableWidget->setColumnWidth(1,120);
    this->ui->tableWidget->setColumnWidth(2,120);
    this->ui->tableWidget->setColumnWidth(3,60);
    this->ui->tableWidget->resizeRowsToContents();
    this->ui->tableWidget->setHorizontalHeaderLabels(titles);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setRelateproperty(QString product)
{
    QList<QPair<QString,QString> >  list = DataHandle::instance().getPropertyList();
    foreach(RESULT p, list){
        if(!product.compare(p.first)){
            QStringList propertys = p.second.split("|");
            this->ui->comboBox_2->clear();
            this->ui->comboBox_2->insertItems(0, propertys);
            break;
        }
    }
}

void MainWindow::addPropertyTowork()
{

    idx = ui->tableWidget->rowCount();
    this->ui->tableWidget->setRowCount(idx+1);
    QString col = this->ui->comboBox->currentText();
    QString col2 = this->ui->comboBox_2->currentText();
    QString col3 = this->ui->comboBox_3->currentText();
    int     idx3 = this->ui->comboBox_3->currentIndex();

    QTableWidgetItem *item = new QTableWidgetItem(col);
    QTableWidgetItem *item2 = new QTableWidgetItem(col2);
    QTableWidgetItem *item3 = new QTableWidgetItem(col3);
    QTableWidgetItem *item4 = new QTableWidgetItem(QString("%1").arg(idx3));
    item->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    item2->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    item3->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    item4->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);

    ui->tableWidget->setItem(idx, 0, item);
    ui->tableWidget->setItem(idx, 1, item2);
    ui->tableWidget->setItem(idx, 2, item3);
    ui->tableWidget->setItem(idx, 3, item4);
    ui->tableWidget->resizeRowsToContents();
    idx++;
}

void MainWindow::delPropertyTowork()
{
    int row = ui->tableWidget->currentRow();
    ui->tableWidget->removeRow(row);
}

void MainWindow::dataProcessTowork()
{
    QString kind = this->ui->comboBox_4->currentText();
    QList<QStringList> fields;
    int     rows = this->ui->tableWidget->rowCount();
    int     cols = this->ui->tableWidget->columnCount();
    for(int i=0; i<rows; i++){
        QStringList field;
        for(int j=0; j<cols; j++){
            QTableWidgetItem *ite = this->ui->tableWidget->item(i,j);
            field.append(ite->text());
        }
        fields.append(field);
    }
    bool  isMulti = false;
    QString targetype = this->ui->comboBox_5->currentText();
    if( targetype.contains("各自生成")){
        isMulti = true;
    }
    if( kind.contains("*.CSV") )
    {
        CVS_Fileparse  cvs(fields,isMulti);
        cvs.parseFilecontext();
    }
    else if( kind.contains("*.RES"))
    {
        RES_Fileparse  res(fields,isMulti);
        res.parseFilecontext();
    }
}
