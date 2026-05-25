#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QMouseEvent>
#include <QEnterEvent>
#include <QPushButton>
#include <map>


enum class StateAdd { Normal, ReadyAdd };

class CellWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor hoverColor READ hoverColor WRITE setHoverColor DESIGNABLE true)
public:
    explicit CellWidget(QWidget* parent = nullptr);
    ~CellWidget() override;

    QColor hoverColor() const { return m_hoverColor; }
    void setHoverColor(const QColor& color) { m_hoverColor = color; update(); }

    enum class WidgetState { Empty, Occupied };
    enum class ShowState   { Normal, Full };

    void setChildWidget(QWidget* widget);
    void clearChildWidget();
    bool isEmpty() const;
    bool isFull();
    void setShowState(ShowState state);
    void setAddState(StateAdd state);

signals:
    void clickedAdd();
    void showModeClicked();
    void deleteClicked();

protected:
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    QWidget*     m_childWidget  = nullptr;
    QPushButton* m_showModeButton;
    QPushButton* m_deleteButton;
    bool         m_isHovered    = false;
    WidgetState  m_state;
    QColor       m_normalColor  = Qt::white;
    QColor       m_hoverColor   = QColor(135, 206, 250, 100);
    ShowState    m_stateView;
    StateAdd     m_addState;
};

class GridWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GridWidget(QWidget* parent = nullptr);
    ~GridWidget() override;

    void addWidgetToCell(const QString& cellName, QWidget* widget);

signals:
    void addReady(const QString& pluginId, const QString& cellName);
    void removeClicked(const QString& pluginId);

public slots:
    void setAddState(const QString& pluginId, StateAdd state);
    void removeWidget(const QString& pluginId);

private slots:
    void addWidgetClicked();
    void removeWidgetClicked();
    void showFullClicked();

private:
    void createGrid(int rows, int cols);
    QString cellNameForPlugin(const QString& pluginId) const;

    QGridLayout*               m_gridLayout = nullptr;
    QString                    m_idAdd;         // plugin id currently being placed
    std::map<QString, QString> cellWidgets;     // cellName → pluginId
};
