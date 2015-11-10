#ifndef REFINELATLNG_H
#define REFINELATLNG_H

#include <QDialog>

namespace Ui {
class RefineLatLng;
}

class RefineLatLng : public QDialog
{
    Q_OBJECT

public:
    explicit RefineLatLng(QWidget *parent = 0);
    ~RefineLatLng();

public slots:
    void    choiceDirPath();
    void    refineSmallsquare();

private:
    Ui::RefineLatLng *ui;
    QString          path;
    QList<QString>   list;

};

#endif // REFINELATLNG_H
