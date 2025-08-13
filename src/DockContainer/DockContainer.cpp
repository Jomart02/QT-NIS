#include "DockContainer.h"
#include <QFile>
#include <QPalette>
#include <QRegularExpression>
#include <QPainterPath>
#include <QApplication>
#include <QStyle>

DockContainer::DockContainer(QString titleName, QWidget* content, QWidget* parent) :
    QDockWidget(parent),
    title(new TitleWidget(titleName, this)),
    content(content)
{
    content->setParent(this);
    setAllowedAreas(Qt::AllDockWidgetAreas);
    setTitleBarWidget(title);
    setWidget(content);

    connect(title, &TitleWidget::show, this, &DockContainer::show_handler);
    connect(title, &TitleWidget::close, this, &DockContainer::close);
    connect(title,&TitleWidget::d_click, this,[=]() {
        content->setVisible(!content->isVisible());
        adjustSize();
    });
}


void DockContainer::paintEvent(QPaintEvent *event)
{
QPainter painter(this);
    painter.setRenderHint(QPainter::RenderHint::Antialiasing); // Включаем сглаживание

    // Рисуем фон с закругленными углами
    QRectF rect(this->rect());
    
    qreal radius = 8; // Радиус закругления углов
    QPainterPath path;

    QColor color;
    painter.setBrush(m_colorBase);
    painter.setPen(Qt::NoPen); // Убираем рамку на этом этапе
    painter.drawRoundedRect(rect, radius, radius);

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

DockContainer::~DockContainer()
{

}
QColor DockContainer::getColorBase() const{
    return m_colorBase;
}
void DockContainer::setColorBase(const QColor &color){
    m_colorBase = color;
    update();
}