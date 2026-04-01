#pragma once

#include <QDockWidget>
#include <QPainter>
#include <QMouseEvent>

class DockContainer : public QDockWidget
{
    Q_OBJECT
    
    Q_PROPERTY(QColor colorBase READ getColorBase WRITE setColorBase DESIGNABLE true)
    Q_PROPERTY(QColor titleColor READ getTitleColor WRITE setTitleColor DESIGNABLE true)
    Q_PROPERTY(int titleHeight READ getTitleHeight WRITE setTitleHeight DESIGNABLE true)
    Q_PROPERTY(int buttonSize READ getButtonSize WRITE setButtonSize DESIGNABLE true)
public:


    enum ButtonFlag{
        NoButton        = 0x0000,
        CloseButton     = 0x0001,
        ToggleButton    = 0x0002,
        FullscreenButton = 0x0004,
        AllButtons      = CloseButton | ToggleButton | FullscreenButton
    };
    Q_ENUM(ButtonFlag)
    using ButtonFlags = QFlags<ButtonFlag>;
    Q_FLAG(ButtonFlags)



    explicit DockContainer(const QString& titleName, QWidget* content, ButtonFlags buttons = ButtonFlags(CloseButton | ToggleButton),QWidget* parent = nullptr);
    
    explicit DockContainer(const QString& titleName, QWidget* content, QWidget* parent);
    ~DockContainer();

    enum NotifyType
    {
        NOTIFY_UNKNOWN = 0,
        NOTIFY_SHOW,
        NOTIFY_CLOSE,
        NOTIFY_FULLSCREEN
    };

    [[nodiscard]] ButtonFlags buttonFlags() const;
    void setButtonFlags(ButtonFlags flags);
    

    [[nodiscard]] QColor getColorBase() const;
    void setColorBase(const QColor &color);

    [[nodiscard]] QColor getTitleColor() const;
    void setTitleColor(const QColor &color);

    [[nodiscard]] int getTitleHeight() const;
    void setTitleHeight(const int &titleHeight);

    [[nodiscard]] int getButtonSize() const;
    void setButtonSize(const int &buttonSize);

    QWidget* getContent();
    void setTitle(const QString& titleName);
    void hideTitleBar();
    void setMakeSignalBeforeClose(bool makeSignal);
    void setContentCollapsed(bool collapsed);

signals:
    void notify(NotifyType what);
    void closeAttempt();

public slots:
    void close();
    void toggleContent();
    void toggleFullscreen();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
    QRect titleRect() const;
    QRect contentRect() const;
    QRect closeButtonRect() const;
    QRect toggleButtonRect() const;
    QRect fullscreenButtonRect() const;

    bool isInTitleArea(const QPoint& pos) const;
    bool isInDragArea(const QPoint& pos) const;
    
    template<ButtonFlag Flag>
    [[nodiscard]] bool hasButton() const {
        return m_buttonFlags.testFlag(Flag);
    }


private:

    QWidget* m_content = nullptr;
    
    bool m_titleActive = true;
    bool m_makeSignalBeforeExit = false;
    bool m_dragging = false;
    bool m_contentCollapsed = false;
    QPoint m_dragStartPos;
    
    int m_titleHeight = 48;
    int m_buttonSize = 36;
    int m_titleMargin = 8;
    bool toggleHovered = false;
    bool closeHovered = false;
    bool fullscreenHovered = false;
    QColor m_colorBase = QColor("#E9F8FF");
    QColor m_titleColor = QColor(Qt::blue);
    bool m_fullscreen = false;
    QString m_titleName;

    ButtonFlags m_buttonFlags = ButtonFlags(CloseButton | ToggleButton);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(DockContainer::ButtonFlags)