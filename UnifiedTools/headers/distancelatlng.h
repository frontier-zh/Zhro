#ifndef DISTANCELATLNG_H
#define DISTANCELATLNG_H

#include <QDialog>

namespace Ui {
class DistanceLatLng;
}

class DistanceLatLng : public QDialog
{
    Q_OBJECT

public:
    explicit DistanceLatLng(QWidget *parent = 0);
    ~DistanceLatLng();

    double rad(double d)
    {
       return d * PI / 180.0;
    }

public slots:

    void    lineDistance();

    double  dotDistance(QString dot1, QString dot2);

    void    obtainGoogleRealLatLng();

private:
    Ui::DistanceLatLng *ui;
    QString  poly;
    double   PI;
    double   EARTH_RADIUS;
};

#endif // DISTANCELATLNG_H
