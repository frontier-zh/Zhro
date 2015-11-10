#include "refinelatlng.h"
#include "ui_refinelatlng.h"
#include <QFileDialog>
#include <QDesktopServices>
#include <QDebug>

RefineLatLng::RefineLatLng(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RefineLatLng), path("")
{
    list.clear();
    ui->setupUi(this);
    QStringList decimal;
    decimal << "0.01" << "0.007" << "0.005";
    ui->comboBox->addItems(decimal);
    connect(ui->button, SIGNAL(clicked()), SLOT(choiceDirPath()));
    connect(ui->button2, SIGNAL(clicked()), SLOT(refineSmallsquare()));

}

RefineLatLng::~RefineLatLng()
{
    delete ui;
}

void
RefineLatLng::choiceDirPath()
{
    ui->lineEdit->setText("");
    QString dir = QFileDialog::getOpenFileName (this, tr("选择目录"),path);
    qDebug() << dir;
    if(dir != ""){
        ui->lineEdit->setText(dir);
        path = dir;
    }

}

void
RefineLatLng::refineSmallsquare()
{
    list.clear();
    ui->result->clear();
    ui->result->setPlainText("frontier_zh");
    if( !ui->lineEdit->text().isEmpty() && !path.isEmpty() ){
        QFile   file(path);
        if( !file.open(QFile::ReadOnly | QFile::Text) ) {
            qDebug() << QString("Open file latlng failed, error: %1 !\n").arg(file.errorString());
            return;
        }
        QTextStream  in(&file);
        QString  line;
        while( !(line = in.readLine()).isEmpty() ){
            line = line.trimmed();
            list << line;
        }
        file.close();
    }

    float deta = ui->comboBox->currentText().toFloat();
    foreach (QString item, list) {
        QStringList elements = item.split(",");
        QMap<QString, QVariant> map;
        QString city = elements.at(0);
        float   lat_min = elements.at(1).toFloat();
        float   lat_max = elements.at(3).toFloat();
        float   lng_min = elements.at(2).toFloat();
        float   lng_max = elements.at(4).toFloat();
        while (lng_min < lng_max) {
            lat_min = elements.at(1).toFloat();
            while(lat_min < lat_max){
                ui->result->insertPlainText( city + "|" + QString("%1,%2;%3,%4\n").arg(lat_min).arg(lng_max).arg(lat_min+deta).arg(lng_max-deta));
                //qDebug() << ui->result->textCursor().position();
                //qDebug() << city << "|" << QString("%1,%2;%3,%4").arg(lat_min).arg(lng_max).arg(lat_min+deta).arg(lng_max-deta);
                lat_min = lat_min + deta;
            }
            lng_max = lng_max - deta;
        }
    }
}
