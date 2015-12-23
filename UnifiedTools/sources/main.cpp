#include "mainwindow.h"
#include <QApplication>
#include "public_define.h"
#include <QtCore>
#include <QTime>
#include "windows.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QString str = "</strong>可分租<br />																																																<strong>开发商";
//    qDebug() <<  str.replace("\t","");
    QString str = "出租求租";
    QByteArray bstr = QTextCodec::codecForName("GB2312")->fromUnicode(str);
     qDebug() << bstr.size();

//    QTime time = QTime::currentTime();
//    qsrand(time.msec()+time.second()*1000);



//    qDebug() << qrand()%48;

//    QString  url = "/../error.asp";
//    url = url.replace("..","");
//    url = url.replace("//","/");
//    qDebug() << url;

//    QString ids = "N(455849959,)";
//        qDebug() << ids.mid(0,ids.length()-1);
//            qDebug() << ids.mid(0,ids.size()-1);
//    QString url = "http://office.sh.2568974.com/shou/3_188618601.html";
//    qDebug() << url.mid(0,url.length()-1);
//    qDebug() << url.mid(0,url.indexOf("/",7));
//    qDebug() << url.indexOf("/");

//      http://www.fangzz.com/village/sales/7
//    QString url = "http://office.sh.2568974.com/shou/3_188618601.html";
//    QRegExp rx(QString("([-_/]\\d+)"));
//    QString urlid = "";
//    int pos = rx.indexIn(url,16);
//    while( (pos = rx.indexIn(url, pos)) != -1 ){
//        urlid = rx.cap(0);
//        pos += rx.matchedLength();
//        qDebug() << pos << ":" << urlid;
//        urlid = urlid.replace("/","");
//        urlid = urlid.replace("-","");
//        urlid = urlid.replace("_","");
//        qDebug() << pos << ":" << urlid;

//        break;
//    }

//    QString fm = "log_caiji.txt";
//    qDebug() << fm.mid(0,fm.indexOf(".")) + QString("_%1") + fm.mid(fm.indexOf("."));

//    QString keyword = " ";
//    QString string = " jkda,, jkdjakfjkd";
//    qDebug() << string.indexOf(keyword,0);

//    QList<int>  list;
//    list << 1 << 8 << 6 << 9 << 8 << 7 << 1 << 2 << 4 << 5 << 7;
//    foreach( int ite, list){
//        if( ite == 8 ){
//            list.removeOne(ite);
//        }
//    }
//    qDebug() << list.size();

//    QString  res  = "name: '\u79C0\u5C71\u571F\u5BB6\u65CF\u82D7\u65CF\u81EA\u6CBB\u53BF'";
//    qDebug() << res;
//    QRegExp  rexp;
//    QString  match = "dat(*)telId";
//    qDebug() << res.contains("data-hotelId=\\\"");
//    rexp.setPattern("data-hotelId=\\\\\"([^*$]*)\\\\\"");
//    rexp.indexIn(res);
//    qDebug() << rexp.cap(0);
//    qDebug() << rexp.cap(1);

//    QString  str = "key1";
//    foreach(QString item, str.split("\r\n")){
//        qDebug() << item;
//    }

//    QMap<QString, QVariant>  map;
//    map.insert("jkdajk", "");
//    qDebug() << map["jkdak"].toInt() << &map;

//    QString  str = "'Search', 'Property', '虹口/凉城/'+classification);";
//    qDebug() << str.indexOf("Property', '");
//    qDebug() <<  str.mid(0,str.indexOf("class"));

//    QString  str = "abcefij>12frontier>345<zhro67890 普洱左耳<jdkajk";
//    if( !str.isEmpty() ){
//        int pos = str.lastIndexOf("<");
//        if( pos != -1 ){
//            str = str.mid(0, pos );
//        }
//        pos = str.indexOf(">", 0 );
//        if( pos != -1 ){
//            str = str.mid(pos + 1);
//        }
//    }
//    qDebug() << str;

//    QString  str = "&nbsp;&nbsp;&nbsp;龄:<b><td><td height=\"30\" align=\"center\"><font color=#6294ac>2011年建</font></td>";
//    qDebug() << str.indexOf("龄:<B><td>",0, Qt::CaseInsensitive);
//    QString  str = "  张 边       \r\n 疆    ";
//    str = str.replace(QRegExp("[\\s]{2,}"), "");
//    qDebug() <<  str;

//    QString  str = "供热好​&zwnj;1.6元/m²・月";
//    wchar_t  *punicode_buf = new  wchar_t[str.size()+1];
//    memset(punicode_buf,0, 2*(str.size() + 1));
//    int     unicode_size = str.toWCharArray(punicode_buf);


//    int ansi_size    = WideCharToMultiByte(CP_ACP,0,punicode_buf,unicode_size,NULL,0,0,0);
//    qDebug() << str.size() << " | " << unicode_size << "|" << ansi_size;
//    char*   pansi_buf   = new char[ansi_size + 1];
//    memset(pansi_buf,0,ansi_size + 1);
//    WideCharToMultiByte(CP_ACP,0,punicode_buf,unicode_size,pansi_buf,ansi_size,0,0);
//    MultiByteToWideChar(CP_ACP,0,pansi_buf,ansi_size,punicode_buf,unicode_size);
//    qDebug() << str.size() << " | " << unicode_size << "|" << ansi_size;

//    qDebug() << QString::fromWCharArray(punicode_buf).mid(0,str.size());

//    QString  str = "&nbsp;200㎡</b>        	<p>厂房面积120平方&nbsp;2190㎡</b>            	<p>厂房 面  积dell";
//    int pos = str.indexOf("  ");
//    while( pos > 0 ){
//        str = str.mid(0, pos+1) + str.mid(pos+1).trimmed();
//        pos = str.indexOf("  ");
//    }
//    qDebug() << str;
//    QString  buff ="3室 2厅 1卫 96㎡       普通住宅            精装修            朝向南";
//    buff = buff.replace(QRegExp("[\\x20]{2,}")," ");
//    qDebug() << buff;

    //QString  buff="九龙城市​&zwnj;&zwj;​&zwnj;&zwj;乐园南500​&zwnj;&zwnj;​";
//    QString buff = "1元/m²・月";
//    qDebug() << QTextCodec::codecForName("GB2312")->fromUnicode(buff);
//    buff = QTextCodec::codecForName("UTF-8")->toUnicode(QTextCodec::codecForName("UTF-8")->fromUnicode(buff));
//    qDebug() << buff;

//    QString buff = "<div>房屋产权：商品房                 建筑面积：96.00㎡                    房屋类型：普通住宅                建筑结构：低层板楼                    装修程度：精装修            土地使用年限：70年                电梯：无                    房源描述：        2 此宅楼层很好，优质3房房通风采光好，户型非常正。3 周边设施完善，几分钟就是市场，医院，银行，酒店，公园，广场等这些都是社区成熟的表现。4 交通便利，5路，6路公交车5 小孩读书，地保幼儿园，二中，实验小学，等学区房6&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbs</div></span><span class=\"bluebtnr\"></span>";
//    buff = QTextCodec::codecForName("GB2312")->toUnicode(QTextCodec::codecForName("GB2312")->fromUnicode(buff));
//    qDebug() << buff.replace("&nbsp","");
    using namespace QsLogging;
    Logger& logger = Logger::instance();
    logger.setLoggingLevel(QsLogging::TraceLevel);
    const QString sLogPath(QDir(a.applicationDirPath()).filePath("log_tools.txt"));

    FileDestination  *fileDestination = new FileDestination(sLogPath, 10*1024*1024, 1 );
    logger.addDestination(fileDestination);
    MainWindow w;
    w.show();

    return a.exec();
}
