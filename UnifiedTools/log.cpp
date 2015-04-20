#include "log.h"
#include <QMutex>
#include <QVector>
#include <QDateTime>
#include <QTextCodec>
#include <QtGlobal>
#include <cassert>
#include <cstdlib>
#include <stdexcept>

namespace QsLogging
{

static const char TraceString[] = "TRACE";
static const char DebugString[] = "DEBUG";
static const char InfoString[]  = "INFO ";
static const char WarnString[]  = "WARN ";
static const char ErrorString[] = "ERROR";
static const char FatalString[] = "FATAL";

static const QString fmtDateTime("yyyy-MM-ddThh:mm:ss.zzz");

static Logger* sInstance = 0;

static const char* LevelToText(Level theLevel)
{
    switch (theLevel) {
        case TraceLevel:
            return TraceString;
        case DebugLevel:
            return DebugString;
        case InfoLevel:
            return InfoString;
        case WarnLevel:
            return WarnString;
        case ErrorLevel:
            return ErrorString;
        case FatalLevel:
            return FatalString;
        case OffLevel:
            return "";
        default: {
            assert(!"bad log level");
            return InfoString;
        }
    }
}

LoggerImpl::LoggerImpl()
    : level(InfoLevel)
{

}


Logger::Logger()
    : d(new LoggerImpl)
{
}

Logger& Logger::instance()
{
    if (!sInstance)
        sInstance = new Logger;

    return *sInstance;
}

void Logger::destroyInstance()
{
    delete sInstance;
    sInstance = 0;
}

// tries to extract the level from a string log message. If available, conversionSucceeded will
// contain the conversion result.
Level Logger::levelFromLogMessage(const QString& logMessage, bool* conversionSucceeded)
{
    if (conversionSucceeded)
        *conversionSucceeded = true;

    if (logMessage.startsWith(QLatin1String(TraceString)))
        return TraceLevel;
    if (logMessage.startsWith(QLatin1String(DebugString)))
        return DebugLevel;
    if (logMessage.startsWith(QLatin1String(InfoString)))
        return InfoLevel;
    if (logMessage.startsWith(QLatin1String(WarnString)))
        return WarnLevel;
    if (logMessage.startsWith(QLatin1String(ErrorString)))
        return ErrorLevel;
    if (logMessage.startsWith(QLatin1String(FatalString)))
        return FatalLevel;

    if (conversionSucceeded)
        *conversionSucceeded = false;
    return OffLevel;
}

Logger::~Logger()
{
    delete d;
    d = 0;
}

void Logger::addDestination(FileDestination *destination)
{
    //assert(destination.data());
    d->dest = destination;
}

void Logger::setLoggingLevel(Level newLevel)
{
    d->level = newLevel;
}

Level Logger::loggingLevel() const
{
    return d->level;
}

//! creates the complete log message and passes it to the logger
void Logger::Helper::writeToLog()
{
    const char* const levelName = LevelToText(level);
    const QString completeMessage(QString("%1 | %2     %3")
                                  .arg(levelName)
                                  .arg(QDateTime::currentDateTime().toString(fmtDateTime))
                                  .arg(buffer)
                                  );

    Logger::instance().enqueueWrite(completeMessage, level);
}

Logger::Helper::~Helper()
{
    try {
        writeToLog();
    }
    catch(std::exception&) {
        // you shouldn't throw exceptions from a sink
        assert(!"exception in logger helper destructor");
        throw;
    }
}

//! directs the message to the task queue or writes it directly
void Logger::enqueueWrite(const QString& message, Level level)
{
    write(message, level);
}

//! Sends the message to all the destinations. The level for this message is passed in case
//! it's useful for processing in the destination.
void Logger::write(const QString& message, Level level)
{
    QMutexLocker lock(&d->logMutex);
    d->dest->write(message);

}

FileDestination::FileDestination(const QString& filePath , qint64 mMaxSize, qint64 mBakcount)
{
    mFile.setFileName(filePath);
    if (!mFile.open(QFile::WriteOnly | QFile::Text | QIODevice::Append))
        qDebug() << "QsLog: could not open log file " << qPrintable(filePath);
    mOutputStream.setDevice(&mFile);
    mOutputStream.setCodec(QTextCodec::codecForName("UTF-8"));
    mMaxSizeInBytes = mMaxSize;
    mBackupsCount   = mBakcount;
    mFileName   = mFile.fileName();

}

void FileDestination::write(const QString& message)
{
    if (mFile.size() > mMaxSizeInBytes ) {
        mOutputStream.setDevice(NULL);
        mFile.close();
        backUp();
        if (!mFile.open(QFile::WriteOnly | QFile::Text | QIODevice::Append))
            qDebug() << "QsLog: could not reopen log file " << qPrintable(mFile.fileName());
        mOutputStream.setDevice(&mFile);
    }

    mOutputStream << message << endl;
    mOutputStream.flush();
}

bool FileDestination::isValid()
{
    return mFile.isOpen();
}

void FileDestination::backUp()
{
    if (!mBackupsCount) {
        if (!QFile::remove(mFileName))
            qDebug() << "QsLog: backup delete failed " << qPrintable(mFileName);
            //std::cerr << "QsLog: backup delete failed " << qPrintable(mFileName);
        return;
    }

     // 1. sure last located
     const QString logNamePattern = mFileName + QString::fromUtf8(".%1");

     int lastExistingBackupIndex = 0;
     for (int i = 1;i <= mBackupsCount;++i) {
         const QString backupFileName = logNamePattern.arg(i);
         if (QFile::exists(backupFileName))
             lastExistingBackupIndex = qMin(i, mBackupsCount - 1);
         else
             break;
     }

     // 2. shift up
     for (int i = lastExistingBackupIndex;i >= 1;--i) {
         const QString oldName = logNamePattern.arg(i);
         const QString newName = logNamePattern.arg(i + 1);
         QFile::remove(newName);
         const bool renamed = QFile::rename(oldName, newName);
         if (!renamed) {
             qDebug() << "QsLog: could not rename backup " << qPrintable(oldName)
                       << " to " << qPrintable(newName);
         }
     }

     // 3. rename current file
     const QString newName = logNamePattern.arg(1);
     if (QFile::exists(newName))
         QFile::remove(newName);
     if (!QFile::rename(mFileName, newName)) {
         qDebug()  << "QsLog: could not rename log " << qPrintable(mFileName)
                   << " to " << qPrintable(newName);
     }
}

} // end namespace

