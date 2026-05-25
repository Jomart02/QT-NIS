#include "DockContainer.h"
#include <QPainterPath>
#include <QFontMetrics>
#include <QCursor>
#include <QVBoxLayout>

DockContainer::DockContainer(const QString& titleName, 
                             QWidget* content, 
                             ButtonFlags buttons,
                             QWidget* parent) :
    QDockWidget(parent),
    m_titleName(titleName),
    m_content(content),
    m_buttonFlags(buttons),
    m_dragging(false),
    m_contentCollapsed(false),
    m_titleActive(true),
    m_makeSignalBeforeExit(false),
    m_fullscreen(false),
    m_titleHeight(48),
    m_buttonSize(36),
    m_titleMargin(8),
    toggleHovered(false),
    closeHovered(false),
    fullscreenHovered(false),
    m_colorBase(QColor("#E9F8FF")),
    m_titleColor(QColor(Qt::blue))
{
    setTitleBarWidget(new QWidget());
    setAllowedAreas(Qt::AllDockWidgetAreas);
    
    QWidget* container = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(container);
    layout->setContentsMargins(6, m_titleHeight, 6, 6);
    layout->setSpacing(0);
    if (m_content) {
        layout->addWidget(m_content);
    }
    
    setWidget(container);
    
    setAttribute(Qt::WA_TranslucentBackground);
    container->setAttribute(Qt::WA_TranslucentBackground);
    
    
    setMouseTracking(true);
    container->setMouseTracking(true);
    
    setMinimumWidth(150);
    setMinimumHeight(m_titleHeight + 50);
}

DockContainer::DockContainer(const QString& titleName, 
                             QWidget* content, 
                             QWidget* parent) :
    DockContainer(titleName, content, ButtonFlags(CloseButton | ToggleButton), parent)
{
}

void DockContainer::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    QRect fullRect = rect();
    int radius = 8;
    
    QRect cRect = contentRect();
    
    if (!m_contentCollapsed && cRect.height() > 0) {
        QPainterPath contentPath;
        contentPath.moveTo(cRect.left(), cRect.top());
        contentPath.lineTo(cRect.left(), cRect.bottom() - radius);
        contentPath.arcTo(cRect.left(), cRect.bottom() - radius*2, radius*2, radius*2, 180, 90);
        contentPath.lineTo(cRect.right() - radius, cRect.bottom());
        contentPath.arcTo(cRect.right() - radius*2, cRect.bottom() - radius*2, radius*2, radius*2, 270, 90);
        contentPath.lineTo(cRect.right(), cRect.top());
        contentPath.lineTo(cRect.left(), cRect.top());
        contentPath.closeSubpath();
        
        QColor bgColor = m_colorBase;
        bgColor.setAlpha(180);
        painter.fillPath(contentPath, bgColor);
        
        
        QLinearGradient contentGrad(0, cRect.top(), 0, cRect.bottom());
        contentGrad.setColorAt(0, QColor(255, 255, 255, 10));
        contentGrad.setColorAt(1, QColor(0, 0, 0, 20));
        painter.fillPath(contentPath, contentGrad);
    }
    
    QRect tRect = titleRect();
    
    QPainterPath titlePath;
    titlePath.moveTo(tRect.left(), tRect.bottom());
    titlePath.lineTo(tRect.left(), tRect.top() + radius);
    titlePath.arcTo(tRect.left(), tRect.top(), radius*2, radius*2, 180, -90);
    titlePath.lineTo(tRect.right() - radius, tRect.top());
    titlePath.arcTo(tRect.right() - radius*2, tRect.top(), radius*2, radius*2, 90, -90);
    titlePath.lineTo(tRect.right(), tRect.bottom());
    titlePath.lineTo(tRect.right(), tRect.bottom());
    titlePath.arcTo(tRect.right() + radius, tRect.bottom(), radius*2, radius*2, 180, -90);
    
    titlePath.closeSubpath();
    
    m_titleColor.setAlpha(240);
    painter.fillPath(titlePath, m_titleColor);
    
    QLinearGradient titleGrad(0, tRect.top(), 0, tRect.bottom());
    titleGrad.setColorAt(0, QColor(255, 255, 255, 80));
    titleGrad.setColorAt(0.5, QColor(255, 255, 255, 40));
    titleGrad.setColorAt(1, QColor(0, 0, 0, 20));
    painter.fillPath(titlePath, titleGrad);
    
    // ЗАГОЛОВОК 
    painter.setPen(QColor(40, 40, 40, 250));
    QFont titleFont = painter.font();
    titleFont.setBold(true);
    titleFont.setPointSize(11);
    painter.setFont(titleFont);
    
    int textX = 12;
    int textY = tRect.center().y() + QFontMetrics(titleFont).ascent()/2 - 2;
    painter.drawText(textX, textY, m_titleName);
    
    // КНОПКИ
    

    if (hasButton<FullscreenButton>()) {
        QRect fsRect = fullscreenButtonRect();
        QPainterPath fsPath;
        fsPath.addRoundedRect(fsRect, 4, 4);
        QColor fsBg = fullscreenHovered ? QColor(255, 255, 255, 100) : QColor(255, 255, 255, 60);
        painter.fillPath(fsPath, fsBg);
        painter.setPen(QPen(QColor(100, 100, 100, 200), 1));
        painter.drawPath(fsPath);
        
        // Иконка ⛶ или ⇱
        painter.setPen(QColor(60, 60, 60, 240));
        painter.setFont(QFont("Arial", 10, QFont::Bold));
        painter.drawText(fsRect, Qt::AlignCenter, m_fullscreen ? "⛶" : "⇱");
    }

    // свернуть/развернуть
    if (hasButton<ToggleButton>()) {
        QRect toggleRect = toggleButtonRect();
        
        QPainterPath togglePath;
        togglePath.addRoundedRect(toggleRect, 4, 4);
        QColor toggleBg = toggleHovered ? QColor(255, 255, 255, 100) : QColor(255, 255, 255, 60);
        painter.fillPath(togglePath, toggleBg);
        painter.setPen(QPen(QColor(100, 100, 100, 200), 1));
        painter.drawPath(togglePath);
        
        painter.setPen(QColor(60, 60, 60, 240));
        painter.setFont(QFont("Arial", 12, QFont::Bold));
        painter.drawText(toggleRect, Qt::AlignCenter, m_contentCollapsed ? "+" : "−");
    }
    // закрыть
    if (hasButton<CloseButton>()) {
        QRect closeRect = closeButtonRect();
        
        QPainterPath closePath;
        closePath.addRoundedRect(closeRect, 4, 4);
        QColor closeBg = closeHovered ? QColor(255, 80, 80, 200) : QColor(255, 100, 100, 150);
        painter.fillPath(closePath, closeBg);
        painter.setPen(QPen(QColor(255, 255, 255, 200), 1));
        painter.drawPath(closePath);
        
        painter.setPen(QColor(255, 255, 255, 240));
        painter.drawText(closeRect, Qt::AlignCenter, "×");
    }

    // QPainterPath borderPath;
    // borderPath.addRoundedRect(fullRect.adjusted(0.5, 0.5, -0.5, -0.5), radius, radius);
    // painter.setPen(QPen(QColor(255, 255, 255, 80), 1));
    // painter.setBrush(Qt::NoBrush);
    // painter.drawPath(borderPath);
}

QRect DockContainer::titleRect() const
{
    return QRect(0, 0, width(), m_titleHeight);
}

QRect DockContainer::contentRect() const
{
    if (m_contentCollapsed ) {
        return QRect(0, m_titleHeight, width(), height() - m_titleHeight);
    }
    
    return QRect(0, m_titleHeight, width(), height() - m_titleHeight);
}

QRect DockContainer::closeButtonRect() const
{
    if (!hasButton<CloseButton>()) return QRect();
    
    int right = width() - m_titleMargin;
    int top = (m_titleHeight - m_buttonSize) / 2;
    return QRect(right - m_buttonSize, top, m_buttonSize, m_buttonSize);
}

QRect DockContainer::toggleButtonRect() const
{
    if (!hasButton<ToggleButton>()) return QRect();
    
    int right;
    if (hasButton<CloseButton>()) {
        right = closeButtonRect().left() - 6;
    } else {
        right = width() - m_titleMargin;
    }
    
    int top = (m_titleHeight - m_buttonSize) / 2;
    return QRect(right - m_buttonSize, top, m_buttonSize, m_buttonSize);
}

QRect DockContainer::fullscreenButtonRect() const
{
    if (!hasButton<FullscreenButton>()) return QRect();
    
    int right;
    if (hasButton<ToggleButton>()) {
        right = toggleButtonRect().left() - 6;
    } else if (hasButton<CloseButton>()) {
        right = closeButtonRect().left() - 6;
    } else {
        right = width() - m_titleMargin;
    }
    
    int top = (m_titleHeight - m_buttonSize) / 2;
    return QRect(right - m_buttonSize, top, m_buttonSize, m_buttonSize);
}

bool DockContainer::isInDragArea(const QPoint& pos) const
{
    QRect tRect = titleRect();
    return tRect.contains(pos) && 
           !closeButtonRect().contains(pos) && 
           !toggleButtonRect().contains(pos) &&
           !fullscreenButtonRect().contains(pos);
}

void DockContainer::mousePressEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton) {
        QDockWidget::mousePressEvent(event);
        return;
    }
    
    QPoint pos = event->pos();
    
    if (hasButton<CloseButton>() && closeButtonRect().contains(pos)) {
        close();
        return;
    }
    
    if (hasButton<ToggleButton>() && toggleButtonRect().contains(pos)) {
        toggleContent();
        return;
    }

    if (hasButton<FullscreenButton>() && fullscreenButtonRect().contains(pos)) {
        toggleFullscreen();
        return;
    }
    
    if (isInDragArea(pos)) {
        m_dragging = true;
        m_dragStartPos = event->globalPosition().toPoint() - frameGeometry().topLeft();
        setCursor(Qt::ClosedHandCursor);
        raise();
        return;
    }
    
    QDockWidget::mousePressEvent(event);
}

void DockContainer::mouseMoveEvent(QMouseEvent* event)
{
    QPoint pos = event->pos();

    bool newCloseHovered = hasButton<CloseButton>() && closeButtonRect().contains(pos);
    bool newToggleHovered = hasButton<ToggleButton>() && toggleButtonRect().contains(pos);
    bool newFullscreenHovered = hasButton<FullscreenButton>() && fullscreenButtonRect().contains(pos);
    
    if (closeHovered != newCloseHovered || 
        toggleHovered != newToggleHovered ||
        fullscreenHovered != newFullscreenHovered) {
        closeHovered = newCloseHovered;
        toggleHovered = newToggleHovered;
        fullscreenHovered = newFullscreenHovered;
        update();
    }
    
   
    if (m_dragging) {
        QPoint newPos = event->globalPos() - m_dragStartPos;
        move(newPos);
        return; 
    }
    
    
    if (isInDragArea(pos)) {
        setCursor(Qt::OpenHandCursor);
    } else if (closeButtonRect().contains(pos) || toggleButtonRect().contains(pos)) {
        setCursor(Qt::PointingHandCursor);
    } else {
        unsetCursor();
    }
    
   
    if (!m_dragging) {
        QDockWidget::mouseMoveEvent(event);
    }
}

void DockContainer::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_dragging && event->button() == Qt::LeftButton) {
        m_dragging = false;
        unsetCursor(); 
    }
    QDockWidget::mouseReleaseEvent(event);
}

void DockContainer::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (isInDragArea(event->pos())) {
        toggleContent();
    } else {
        QDockWidget::mouseDoubleClickEvent(event);
    }
}

void DockContainer::resizeEvent(QResizeEvent* event)
{
    QDockWidget::resizeEvent(event);
}


void DockContainer::toggleFullscreen()
{
    // m_fullscreen = !m_fullscreen;
    
    // if (m_fullscreen) {
        
    //     m_normalGeometry = geometry();
    //     setFloating(true);
    //     showFullScreen();
    // } else {
    //     showNormal();
    //     if (m_normalGeometry.isValid()) {
    //         setGeometry(m_normalGeometry);
    //     }
    // }
    
    // emit notify(NOTIFY_FULLSCREEN);
}

void DockContainer::setButtonFlags(ButtonFlags flags)
{
    if (m_buttonFlags != flags) {
        m_buttonFlags = flags;
        update(); 
    }
}

void DockContainer::toggleContent()
{
    m_contentCollapsed = !m_contentCollapsed;
    
    if (m_content) {
        m_content->setVisible(!m_contentCollapsed);
        
        if (m_contentCollapsed) {
            setMaximumHeight(m_titleHeight);
            resize(width(), m_titleHeight);
        } else {
            setMaximumHeight(QWIDGETSIZE_MAX);
            if (height() <= m_titleHeight) {
                resize(width(), m_titleHeight + 200);
            }
        }
    }
    update();
}

QWidget* DockContainer::getContent()
{
    return m_content;
}

void DockContainer::setMakeSignalBeforeClose(bool makeSignal)
{
    m_makeSignalBeforeExit = makeSignal;
}

void DockContainer::hideTitleBar() 
{
    m_titleActive = false;
    m_titleHeight = 0;
    update();
}

void DockContainer::setTitle(const QString& titleName)
{
    m_titleName = titleName;
    update();
}


void DockContainer::setContentCollapsed(bool collapsed)
{
    if (m_contentCollapsed != collapsed) {
        toggleContent();
    }
}

void DockContainer::close()
{
    emit notify(NotifyType::NOTIFY_CLOSE);
    if (!m_makeSignalBeforeExit) {
        deleteLater();
    } else {
        emit closeAttempt();
    }
}

QColor DockContainer::getColorBase() const
{
    return m_colorBase;
}

void DockContainer::setColorBase(const QColor &color)
{
    m_colorBase = color;
    update();
}

QColor DockContainer::getTitleColor() const{
    return m_titleColor;
}
void DockContainer::setTitleColor(const QColor &color){
    m_titleColor = color;
    update();
}

int DockContainer::getTitleHeight() const{
    return m_titleHeight;
}
void DockContainer::setTitleHeight(const int &titleHeight){
    if (m_titleHeight == titleHeight) return;
    
    m_titleHeight = titleHeight;
    
    QWidget* container = widget();
    if (container) {
        QLayout* layout = container->layout();
        if (layout) {
            layout->setContentsMargins(6, m_titleHeight, 6, 6);
        }
    }
    
    setMinimumHeight(m_titleHeight + 50);
    update();
}

int DockContainer::getButtonSize() const{
    return m_buttonSize;
}
void DockContainer::setButtonSize(const int &buttonSize){
    m_buttonSize = buttonSize;
    setMinimumHeight(m_titleHeight + 50);
    update();
}


DockContainer::~DockContainer()
{
}