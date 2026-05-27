#pragma once

#include <QWidget>
#include "GridLayoutDef.h"


class LayoutPreviewButton : public QWidget {
    Q_OBJECT
public:
    explicit LayoutPreviewButton(const GridLayoutDef& def, QWidget* parent = nullptr);

    void setChecked(bool checked);
    bool isChecked() const { return m_checked; }

    QSize sizeHint() const override;

signals:
    void clicked();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    GridLayoutDef m_def;
    bool          m_checked = false;
    bool          m_hovered = false;
};
