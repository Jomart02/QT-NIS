#include "GridWidget.h"

#include <QLayoutItem>
#include <QMouseEvent>
#include <QPainter>

// ---------------------------------------------------------------------------
// CellWidget
// ---------------------------------------------------------------------------

CellWidget::CellWidget(QWidget* parent)
    : QWidget(parent)
    , m_state(WidgetState::Empty)
    , m_stateView(ShowState::Normal)
    , m_addState(StateAdd::Normal)
{
    setMouseTracking(true);
    setLayout(new QGridLayout(this));

    m_showModeButton = new QPushButton("✎", this);
    m_deleteButton   = new QPushButton("✖", this);
    m_showModeButton->setFixedSize(48, 48);
    m_deleteButton->setFixedSize(48, 48);
    m_showModeButton->hide();
    m_deleteButton->hide();

    connect(m_showModeButton, &QPushButton::clicked, this, &CellWidget::showModeClicked);
    connect(m_deleteButton,   &QPushButton::clicked, this, &CellWidget::deleteClicked);
}

CellWidget::~CellWidget()
{
    delete m_childWidget;
}

void CellWidget::setChildWidget(QWidget* widget)
{
    if (m_childWidget)
        delete m_childWidget;

    m_childWidget = widget;
    if (m_childWidget) {
        m_childWidget->setParent(this);
        layout()->addWidget(m_childWidget);
        m_state     = WidgetState::Occupied;
        m_isHovered = false;
        m_showModeButton->show();
        m_deleteButton->show();
        m_showModeButton->raise();
        m_deleteButton->raise();
        update();
    }
}

void CellWidget::clearChildWidget()
{
    delete m_childWidget;
    m_childWidget = nullptr;
    m_state       = WidgetState::Empty;
    m_isHovered   = false;
    m_showModeButton->hide();
    m_deleteButton->hide();
    update();
}

bool CellWidget::isEmpty() const { return m_state     == WidgetState::Empty; }
bool CellWidget::isFull()        { return m_stateView == ShowState::Full; }

void CellWidget::setShowState(ShowState state) { m_stateView = state; }

void CellWidget::setAddState(StateAdd state)
{
    m_addState  = state;
    m_isHovered = false;
    update();
}

void CellWidget::enterEvent(QEnterEvent* event)
{
    if (isEmpty() && m_addState == StateAdd::ReadyAdd)
        m_isHovered = true, update();
    QWidget::enterEvent(event);
}

void CellWidget::leaveEvent(QEvent* event)
{
    if (isEmpty() && m_addState == StateAdd::ReadyAdd)
        m_isHovered = false, update();
    QWidget::leaveEvent(event);
}

void CellWidget::mousePressEvent(QMouseEvent* event)
{
    if (isEmpty() && m_addState == StateAdd::ReadyAdd)
        emit clickedAdd();
    QWidget::mousePressEvent(event);
}

void CellWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(m_normalColor);
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect());

    if (m_isHovered && isEmpty()) {
        painter.setBrush(QBrush(m_hoverColor));
        QPen pen(QColor(30, 144, 255));
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        painter.setPen(pen);
        painter.drawRect(rect().adjusted(2, 2, -2, -2));

        QPolygon triangle;
        triangle << QPoint(10, 10) << QPoint(20, 10) << QPoint(15, 15);
        painter.setBrush(QColor(70, 130, 180));
        painter.setPen(Qt::NoPen);
        painter.drawPolygon(triangle);
    }
}

void CellWidget::resizeEvent(QResizeEvent* event)
{
    const int margin  = 5;
    const int spacing = 5;
    m_showModeButton->move(
        width() - m_showModeButton->width() - margin - m_deleteButton->width() - spacing,
        margin);
    m_deleteButton->move(width() - m_deleteButton->width() - margin, margin);
    QWidget::resizeEvent(event);
}

// ---------------------------------------------------------------------------
// GridWidget
// ---------------------------------------------------------------------------

GridWidget::GridWidget(QWidget* parent) : QWidget(parent)
{
    createGrid(2, 2);
}

GridWidget::~GridWidget() = default;

void GridWidget::createGrid(int rows, int cols)
{
    m_gridRows = rows;
    m_gridCols = cols;
    m_fullCell = nullptr;
    m_cellDefs.clear();

    delete m_gridLayout;
    m_gridLayout = new QGridLayout(this);
    m_gridLayout->setSpacing(0);
    m_gridLayout->setContentsMargins(0, 0, 0, 0);

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            auto* cell = new CellWidget(this);
            cell->setObjectName("CELL" + QString::number(row) + QString::number(col));
            m_gridLayout->addWidget(cell, row, col);
            m_cellDefs[cell] = {row, col, 1, 1};
            connect(cell, &CellWidget::clickedAdd,      this, &GridWidget::addWidgetClicked);
            connect(cell, &CellWidget::deleteClicked,   this, &GridWidget::removeWidgetClicked);
            connect(cell, &CellWidget::showModeClicked, this, &GridWidget::showFullClicked);
        }
    }
    setLayout(m_gridLayout);
}

void GridWidget::addWidgetToCell(const QString& cellName, QWidget* widget)
{
    auto* cell = findChild<CellWidget*>(cellName);
    if (!cell) return;

    cell->setChildWidget(widget);
    cellWidgets[cellName] = m_idAdd;

    for (auto* c : findChildren<CellWidget*>())
        c->setAddState(StateAdd::Normal);
    m_idAdd.clear();
}

void GridWidget::addWidgetClicked()
{
    auto* cell = qobject_cast<CellWidget*>(sender());
    if (cell && !m_idAdd.isEmpty())
        emit addReady(m_idAdd, cell->objectName());
}

void GridWidget::setAddState(const QString& pluginId, StateAdd state)
{
    for (auto* c : findChildren<CellWidget*>())
        c->setAddState(state);
    m_idAdd = (state == StateAdd::ReadyAdd) ? pluginId : QString{};
}

void GridWidget::exitFullScreenIfNeeded(CellWidget* cell)
{
    if (!cell || !cell->isFull()) return;

    // Возвращаем ячейку на исходную позицию в layout
    const GridCellDef& d = m_cellDefs.value(cell);
    m_gridLayout->removeWidget(cell);
    m_gridLayout->addWidget(cell, d.row, d.col, d.rowSpan, d.colSpan);

    // Показываем все ячейки, которые были скрыты
    for (auto* c : findChildren<CellWidget*>())
        c->setVisible(true);

    cell->setShowState(CellWidget::ShowState::Normal);
    m_fullCell = nullptr;
}

void GridWidget::removeWidgetClicked()
{
    auto* cell = qobject_cast<CellWidget*>(sender());
    if (!cell) return;

    auto it = cellWidgets.find(cell->objectName());
    if (it == cellWidgets.end()) return;

    emit removeClicked(it->second);
    // Сначала выходим из full-screen (если нужно), потом очищаем содержимое
    exitFullScreenIfNeeded(cell);
    cell->clearChildWidget();
    cellWidgets.erase(it);
}

void GridWidget::removeWidget(const QString& pluginId)
{
    const QString name = cellNameForPlugin(pluginId);
    if (name.isEmpty()) return;

    auto* cell = findChild<CellWidget*>(name);
    if (!cell) return;

    exitFullScreenIfNeeded(cell);
    cell->clearChildWidget();
    cellWidgets.erase(name);
}

void GridWidget::showFullClicked()
{
    auto* cell = qobject_cast<CellWidget*>(sender());
    if (!cell) return;

    const bool needFull = !cell->isFull();

    if (needFull) {
        // ── Переход в полноэкранный режим ────────────────────────────────────
        // Проблема: если у ячейки colSpan < m_gridCols (в асимметричной раскладке),
        // простое скрытие соседей не даёт ей расшириться на всю ширину GridWidget,
        // поскольку QGridLayout ограничивает виджет его зарегистрированными столбцами.
        //
        // Решение: временно перерегистрируем ячейку на весь диапазон (0, 0, rows, cols).
        // Это гарантирует полный охват независимо от исходного span.
        m_gridLayout->removeWidget(cell);
        m_gridLayout->addWidget(cell, 0, 0, m_gridRows, m_gridCols);

        for (auto* c : findChildren<CellWidget*>())
            if (c != cell) c->setVisible(false);

        cell->setShowState(CellWidget::ShowState::Full);
        m_fullCell = cell;

    } else {
        exitFullScreenIfNeeded(cell);
    }
}

QString GridWidget::cellNameForPlugin(const QString& pluginId) const
{
    for (const auto& [cell, id] : cellWidgets)
        if (id == pluginId) return cell;
    return {};
}

void GridWidget::applyLayout(const GridLayoutDef& def)
{
    // Сохраняем снимок ID размещённых плагинов до очистки карты
    QList<QString> placedIds;
    placedIds.reserve(static_cast<int>(cellWidgets.size()));
    for (const auto& [cellName, pluginId] : cellWidgets)
        placedIds.append(pluginId);

    // Сбрасываем всё состояние
    cellWidgets.clear();
    m_idAdd.clear();
    m_cellDefs.clear();
    m_fullCell = nullptr;
    m_gridRows = def.rows;
    m_gridCols = def.cols;

    // Удаляем старые ячейки
    qDeleteAll(findChildren<CellWidget*>(QString{}, Qt::FindDirectChildrenOnly));

    // Перестраиваем компоновку
    delete m_gridLayout;
    m_gridLayout = new QGridLayout(this);
    m_gridLayout->setSpacing(0);
    m_gridLayout->setContentsMargins(0, 0, 0, 0);

    for (const GridCellDef& c : def.cells) {
        auto* cell = new CellWidget(this);
        cell->setObjectName(QStringLiteral("CELL%1%2").arg(c.row).arg(c.col));
        m_gridLayout->addWidget(cell, c.row, c.col, c.rowSpan, c.colSpan);
        m_cellDefs[cell] = c;  // сохраняем для восстановления после full-screen
        connect(cell, &CellWidget::clickedAdd,      this, &GridWidget::addWidgetClicked);
        connect(cell, &CellWidget::deleteClicked,   this, &GridWidget::removeWidgetClicked);
        connect(cell, &CellWidget::showModeClicked, this, &GridWidget::showFullClicked);
    }
    setLayout(m_gridLayout);

    for (const QString& pluginId : placedIds)
        emit removeClicked(pluginId);
}
