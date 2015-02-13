#ifndef CVS_FILEPARSE_H
#define CVS_FILEPARSE_H

#include <QObject>
#include <QStringList>

typedef QPair<QString,QString>  RESULT;

class CVS_Fileparse : public QObject
{
    Q_OBJECT
public:
    explicit CVS_Fileparse(QObject *parent = 0);
    CVS_Fileparse(QList<QStringList> fields, bool isMulti);

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

#endif // CVS_FILEPARSE_H
