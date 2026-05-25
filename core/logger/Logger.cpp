#include "Logger.h"

#include <QDateTime>
#include <QFile>
#include <QFileInfo>
#include <QMutex>
#include <QMutexLocker>
#include <QTextStream>
#include <iostream>

static QFile  s_logFile;
static QMutex s_mutex;

Logger& Logger::instance()
{
    static Logger inst;
    return inst;
}

Logger::Logger(QObject* parent)
    : QObject(parent)
{
    s_logFile.setFileName("app.log");
    s_logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
}

void Logger::install()
{
    instance();
    qInstallMessageHandler(Logger::messageHandler);
}

void Logger::messageHandler(QtMsgType type,
                            const QMessageLogContext& ctx,
                            const QString& msg)
{
    static const char* labels[] = {"DBG", "WRN", "CRT", "FTL", "INF"};
    const char* label = (type <= QtInfoMsg) ? labels[type] : "???";

    const QString timestamp = QDateTime::currentDateTime()
                                  .toString("yyyy-MM-dd hh:mm:ss.zzz");

    QString location;
    if (ctx.file)
        location = QString("%1:%2").arg(QFileInfo(ctx.file).fileName()).arg(ctx.line);

    const QString line = location.isEmpty()
        ? QString("[%1][%2] %3").arg(timestamp, label, msg)
        : QString("[%1][%2][%3] %4").arg(timestamp, label, location, msg);

    QMutexLocker lock(&s_mutex);

    std::cerr << line.toStdString() << '\n';

    if (s_logFile.isOpen()) {
        QTextStream ts(&s_logFile);
        ts << line << '\n';
        ts.flush();
    }

    QMetaObject::invokeMethod(&Logger::instance(), "logged",
                              Qt::QueuedConnection,
                              Q_ARG(QtMsgType, type),
                              Q_ARG(QString, line));
}
