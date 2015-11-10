#ifndef DATACOMPARISON_H
#define DATACOMPARISON_H

#include <QDialog>
#include <QtCore>

namespace Ui {
class DataComparison;
}

class DataComparison : public QDialog
{
    Q_OBJECT

public:
    explicit DataComparison(QWidget *parent = 0);
    ~DataComparison();

private slots:
    void    comparisonTowork();
    void    loadthreadFinished();
    void    onSliderMovLoading(int position);
    void    stopthreadFeedback(QString feedback);

private:
    Ui::DataComparison  *ui;
    int                 idx;
    QList<QStringList>  result;
    int                 previous;
    bool                isload;
};

#endif // DATACOMPARISON_H
