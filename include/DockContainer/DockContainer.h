#pragma once

#include <QDockWidget>

#include "TitleWidget.h"
#include <QPainter>


class DockContainer : public QDockWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor colorBase READ getColorBase WRITE setColorBase DESIGNABLE true)
public:
    DockContainer(QString titleName, QWidget* content, QWidget* parent = nullptr);
    ~DockContainer();

    enum NotifyType
    {
        NOTIFY_UNKNOWN = 0,
        NOTIFY_SHOW,
        NOTIFY_CLOSE
    };

    QColor getColorBase() const;
    void setColorBase(const QColor &color);
protected:
    void paintEvent(QPaintEvent *event);
public:
    QWidget* getContent();
    void setTitle(const QString& titleName);
    void setTitleRaise(bool flag);
    void hideTitleBar();
    void setMakeSignalBeforeClose(bool makerSignal);
signals:
    void notify(NotifyType what);
    void closeAttempt();
public slots:
    void close();

private slots:
    void show_handler();
private:
    TitleWidget* title;
    QWidget* content;
    bool titleActive  = true;
    bool makeSignalBeforeExit = false;
    QColor m_colorBase = QColor("#E9F8FF");
};
