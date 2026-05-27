#pragma once

#include <QWidget>
#include "GridLayoutDef.h"

class QScrollArea;
class QPushButton;
class LayoutPreviewButton;

class LayoutGalleryWidget : public QWidget {
    Q_OBJECT
public:
    explicit LayoutGalleryWidget(const QList<GridLayoutDef>& presets,
                                  QWidget* parent = nullptr);

    void setCurrentIndex(int index);

signals:
    void layoutSelected(int index);
    void configureRequested();

private slots:
    void onScrollLeft();
    void onScrollRight();
    void onScrollValueChanged();

private:

    static constexpr int kVisibleCount = 2;

    QScrollArea*                m_scrollArea   = nullptr;
    QList<LayoutPreviewButton*> m_buttons;
    QPushButton*                m_leftArrow    = nullptr;
    QPushButton*                m_rightArrow   = nullptr;
    QPushButton*                m_configureBtn = nullptr;
};
