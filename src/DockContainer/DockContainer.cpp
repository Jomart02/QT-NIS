#include "DockContainer.h"
#include <QFile>
#include <QPalette>
#include <QRegularExpression>
#include <QPainterPath>
#include <QApplication>
#include <QStyle>

DockContainer::DockContainer(QString titleName, QWidget* content, QWidget* parent,Qt::WindowType typeWindow) :
    QDockWidget(parent,typeWindow),
    title(new TitleWidget(titleName, this)),
    content(content)
{
    content->setParent(this);
    //content->setAutoFillBackground(true);
    setAllowedAreas(Qt::AllDockWidgetAreas);
    setTitleBarWidget(title);
    setWidget(content);

}


void DockContainer::paintEvent(QPaintEvent *event)
{

}

QWidget* DockContainer::getContent()
{
    return content;
}
void DockContainer::setMakeSignalBeforeClose(bool makeSignal)
{
    makeSignalBeforeExit = makeSignal;
}

void DockContainer::hideTitleBar() {
    this->setTitleBarWidget(new QWidget());
    titleActive = false;
    update();
}

void DockContainer::setTitle(const QString& titleName)
{
    title->setTitle(titleName);
}

void DockContainer::show_handler()
{
	emit notify(NotifyType::NOTIFY_SHOW);
}

void DockContainer::close()
{
    emit notify(NotifyType::NOTIFY_CLOSE);
    if(!makeSignalBeforeExit) deleteLater();
    else closeAttempt();
}

QSize DockContainer::getTitleSize()
{
    return title->size();
}

DockContainer::~DockContainer()
{

}
