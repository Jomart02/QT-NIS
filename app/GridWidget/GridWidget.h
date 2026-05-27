#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QMap>
#include <QMouseEvent>
#include <QEnterEvent>
#include <QPushButton>
#include <map>

#include "GridLayoutDef.h"


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

    // Применяет новую раскладку. Все ранее размещённые виджеты удаляются;
    // для каждого из них испускается сигнал removeClicked.
    void applyLayout(const GridLayoutDef& def);

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

    // Если cell находится в full-screen режиме — возвращает её на исходную позицию
    // в layout, делает все остальные ячейки видимыми и сбрасывает m_fullCell.
    // Если cell не в full-screen — ничего не делает.
    void exitFullScreenIfNeeded(CellWidget* cell);

    QGridLayout*               m_gridLayout = nullptr;
    QString                    m_idAdd;              // id плагина, ожидающего размещения
    std::map<QString, QString> cellWidgets;          // cellName → pluginId

    // Исходные позиции ячеек в сетке (нужны для восстановления после full-screen).
    // Ключ — указатель на CellWidget; значение — его GridCellDef при создании.
    QMap<CellWidget*, GridCellDef> m_cellDefs;

    int         m_gridRows = 0;    // размер текущей сетки по строкам
    int         m_gridCols = 0;    // размер текущей сетки по столбцам
    CellWidget* m_fullCell = nullptr; // ячейка в режиме full-screen (nullptr если нет)
};
