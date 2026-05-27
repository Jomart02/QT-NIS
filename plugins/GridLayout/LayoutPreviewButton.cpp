#include "LayoutPreviewButton.h"

#include <QMouseEvent>
#include <QPainter>

// Размеры кнопки в пикселях
static constexpr int kW       = 54;   // ширина
static constexpr int kH       = 62;   // высота
static constexpr int kPad     = 5;    // отступ вокруг превью

static constexpr int kGridW   = kW - kPad * 2;
static constexpr int kGridH   = kH - kPad * 2 - 2;

LayoutPreviewButton::LayoutPreviewButton(const GridLayoutDef& def, QWidget* parent)
    : QWidget(parent)
    , m_def(def)
{
    setFixedSize(kW, kH);
    setCursor(Qt::PointingHandCursor);
    setToolTip(def.displayName);
}

QSize LayoutPreviewButton::sizeHint() const
{
    return {kW, kH};
}

void LayoutPreviewButton::setChecked(bool checked)
{
    if (m_checked == checked) return;
    m_checked = checked;
    update();
}

void LayoutPreviewButton::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, false);

    // Фон и рамка кнопки 
    if (m_checked) {
        p.fillRect(rect(), QColor(0x2a, 0x3f, 0x6a));
        p.setPen(QPen(QColor(0x4a, 0x8a, 0xff), 1));
        p.drawRect(rect().adjusted(0, 0, -1, -1));
    } else if (m_hovered) {
        p.fillRect(rect(), QColor(0x1e, 0x2d, 0x4a));
        p.setPen(QPen(QColor(0x3a, 0x5a, 0x9a), 1));
        p.drawRect(rect().adjusted(0, 0, -1, -1));
    }

    //Миниатюра ячеек 
    const QRect gridRect(kPad, kPad, kGridW, kGridH);

    const double cellW = static_cast<double>(kGridW) / m_def.cols;
    const double cellH = static_cast<double>(kGridH) / m_def.rows;

    const QColor fillColor  (0x0d, 0x1a, 0x30);  // заливка ячейки
    const QColor borderColor(0x3a, 0x5a, 0x9a);  // рамка ячейки

    p.setPen(QPen(borderColor, 1));

    for (const GridCellDef& c : m_def.cells) {
        const int x = gridRect.x() + qRound(c.col     * cellW);
        const int y = gridRect.y() + qRound(c.row     * cellH);
        const int w = qRound(c.colSpan * cellW);
        const int h = qRound(c.rowSpan * cellH);

        p.fillRect(x + 1, y + 1, w - 2, h - 2, fillColor);
        p.drawRect(x, y, w - 1, h - 1);
    }

}

void LayoutPreviewButton::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
        emit clicked();
    QWidget::mousePressEvent(event);
}

void LayoutPreviewButton::enterEvent(QEnterEvent* event)
{
    m_hovered = true;
    update();
    QWidget::enterEvent(event);
}

void LayoutPreviewButton::leaveEvent(QEvent* event)
{
    m_hovered = false;
    update();
    QWidget::leaveEvent(event);
}
