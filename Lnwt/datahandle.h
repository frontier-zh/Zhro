#ifndef DATAHANDLE_H
#define DATAHANDLE_H

#include <QObject>
#include <QtCore>
#include <QDebug>
#include <QStringList>
#include <QPair>
#include <QFile>
#include <QTextStream>

typedef QPair<QString,QString>  RESULT;

class DataHandle : public QObject
{
    Q_OBJECT
public:
    explicit DataHandle(QObject *parent = 0);
    static DataHandle& instance();
    QStringList& getProductList();
    QStringList& getStandardList();
    QList<RESULT>& getPropertyList();

signals:

public slots:

private:
    QStringList  product;
    QStringList  standard;
    QList<RESULT> property;

};

#endif // DATAHANDLE_H
