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
    QStringList& getHostList();
    QStringList& getImpHostList();
    QStringList& getImpDbList();
    QStringList& getImpUserList();
    QStringList& getImpPassList();
    QStringList& getExpHostList();
    QStringList& getExpDbList();
    QStringList& getExpUserList();
    QStringList& getExpPassList();


signals:

public slots:

private:
    QStringList  imp_host;
    QStringList  imp_name;
    QStringList  imp_user;
    QStringList  imp_pass;
    QStringList  exp_host;
    QStringList  exp_name;
    QStringList  exp_user;
    QStringList  exp_pass;

};

#endif // DATAHANDLE_H
