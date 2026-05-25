#pragma once

#include <QObject>
#include <QString>
#include <QtMessageHandler>


class Logger : public QObject
{
    Q_OBJECT
public:
    static Logger& instance();

    static void install();

signals:
    void logged(QtMsgType type, const QString& message);

private:
    explicit Logger(QObject* parent = nullptr);

    static void messageHandler(QtMsgType type,
                               const QMessageLogContext& ctx,
                               const QString& msg);
};
