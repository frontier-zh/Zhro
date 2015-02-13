#ifndef RES_FILEPARSE_H
#define RES_FILEPARSE_H

#include <QObject>
#include <QStringList>

typedef QPair<QString,QString>  RESULT;

class RES_Fileparse : public QObject
{
    Q_OBJECT
public:
    explicit RES_Fileparse(QObject *parent = 0);
    RES_Fileparse(QList<QStringList> fields, bool isMulti);

    void    parseFilecontext();

signals:

public slots:
private:
    bool        ismulti;
    int         lastbegin;
    QString     lastproduct;
    QList<QString>      files;
    QList<QStringList>  fields;

};

#endif // RES_FILEPARSE_H
