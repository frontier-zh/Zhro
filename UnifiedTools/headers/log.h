#ifndef CJ_LOG_H
#define CJ_LOG_H

#include <QDebug>
#include <QString>
#include <QMutex>
#include <QFile>


namespace QsLogging
{
enum Level
{
    TraceLevel = 0,
    DebugLevel,
    InfoLevel,
    WarnLevel,
    ErrorLevel,
    FatalLevel,
    OffLevel
};
///////
class FileDestination
{
public:
    FileDestination(const QString& filePath, qint64 mMaxSize, qint64 mBakcount);
    void    write(const QString& message);
    bool    isValid();
    void    backUp();

private:
    QFile   mFile;
    QTextStream mOutputStream;
    QString mFileName;
    qint64  mMaxSizeInBytes;
    int     mBackupsCount;
};

///////
class LoggerImpl
{
public:
    LoggerImpl();

    QMutex  logMutex;
    Level   level;
    FileDestination *dest;
};

///////
class  Logger
{
public:
    static Logger& instance();
    static void destroyInstance();
    static Level levelFromLogMessage(const QString& logMessage, bool* conversionSucceeded = 0);

    ~Logger();

    //! Adds a log message destination. Don't add null destinations.
    void addDestination(FileDestination *destination);
    //! Logging at a level < 'newLevel' will be ignored
    void setLoggingLevel(Level newLevel);
    //! The default level is INFO
    Level loggingLevel() const;

    //! The helper forwards the streaming to QDebug and builds the final
    //! log message.
    class  Helper
    {
    public:
        explicit Helper(Level logLevel) :
            level(logLevel),
            qtDebug(&buffer) {}
        ~Helper();
        QDebug& stream(){ return qtDebug; }

    private:
        void writeToLog();

        Level level;
        QString buffer;
        QDebug qtDebug;
    };

private:
    Logger();
    Logger(const Logger&);            // not available
    Logger& operator=(const Logger&); // not available

    void enqueueWrite(const QString& message, Level level);
    void write(const QString& message, Level level);

    LoggerImpl* d;

};

} // end namespace

#define QLOG_TRACE() \
        QsLogging::Logger::Helper(QsLogging::TraceLevel).stream()
#define QLOG_DEBUG() \
        QsLogging::Logger::Helper(QsLogging::DebugLevel).stream()
#define QLOG_INFO()  \
        QsLogging::Logger::Helper(QsLogging::InfoLevel).stream()
#define QLOG_WARN()  \
        QsLogging::Logger::Helper(QsLogging::WarnLevel).stream()
#define QLOG_ERROR() \
        QsLogging::Logger::Helper(QsLogging::ErrorLevel).stream()
#define QLOG_FATAL() \
        QsLogging::Logger::Helper(QsLogging::FatalLevel).stream()

#endif // CJ_LOG_H
