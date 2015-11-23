#include "basemapwidget.h"
#include <QHBoxLayout>
#include <QGraphicsScene>
#include "flowdatastate.h"
#include <QFile>
#include <QIODevice>
#include <QTextStream>

BaseMapWidget::BaseMapWidget(QWidget *parent) :
    BasePluginWidget(parent), gview(NULL)
{
    initUi();
    mCitys.clear();
    QFile  file("1.txt");
    if( !file.open(QIODevice::ReadOnly|QIODevice::Text) ){
        qDebug() << "Open pv_data file failed!";
    }else{
        QTextStream  out(&file);
        QString     line="";
        while( ! (line = out.readLine()).isEmpty() )
        {
            //qDebug() <<  line;
            QPoint  pos;
            QString key = line.split(",").at(0);
            pos.setX(line.split(",").at(1).toFloat());
            pos.setY(line.split(",").at(2).toFloat());
            mCitys.insert(key,pos);
        }
    }
    file.close();

    QFile  file2("railway.txt");
    if( !file2.open(QIODevice::ReadOnly|QIODevice::Text) ){
        qDebug() << "Open pv_data file failed!";
    }else{
        QTextStream  in(&file2);
        QString     line="";
        while( ! (line = in.readLine()).isEmpty() )
        {
            //qDebug() <<  line;
            QString sColor = line.split("|").at(0);
            QString sPoly = line.split("|").at(1);
            QPainterPath path;
            QStringList lPoints = sPoly.split(";");
            for( int i = 0; i < lPoints.size(); i++ ){
                QString  sPoint = lPoints.at(i);
                if( i == 0 ){
                    path.moveTo(sPoint.split(",").at(1).toFloat(),
                                sPoint.split(",").at(2).toFloat());
                }else{
                    path.lineTo(sPoint.split(",").at(1).toFloat(),
                                sPoint.split(",").at(2).toFloat());
                }
            }
            mPolys.insert(sColor,path);
        }
    }
    file2.close();

    curState();
}

void BaseMapWidget::initUi()
{
    QSizePolicy sp(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->setSizePolicy(sp);
    this->resize(960, 540);
    this->setMinimumSize(960, 540);
    this->setMaximumSize(960, 540);

    this->gview = new QGraphicsView(this);
    this->gview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->gview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->gview->resize(960, 540);
    this->gview->setMinimumSize(960, 540);
    this->gview->setMaximumSize(960, 540);

    this->gview->setFocusPolicy(Qt::StrongFocus);

    this->setFocusProxy(this->gview);
    this->setFocus();

    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->setSpacing(0);
    hbox->setMargin(0);

    hbox->addWidget(this->gview);

    QGraphicsScene *scn = new QGraphicsScene(0.0, 0.0, 960, 540, this);
    scn->setBackgroundBrush(QBrush(QPixmap("bg_map.png")));
    this->gview->setScene(scn);
}

void BaseMapWidget::curState()
{
    FlowDataState *fds = new FlowDataState(this);
    fds->init( gview->scene());
    fds->setData(mCitys, mPolys);
}
