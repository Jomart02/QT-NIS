#include "LayoutGalleryWidget.h"
#include "LayoutPreviewButton.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>

// Размер одной кнопки-превью 
static constexpr int kBtnW    = 54;
static constexpr int kBtnH    = 62;
static constexpr int kSpacing = 2;   // отступ между кнопками в контейнере
static constexpr int kArrowW  = 16;  // ширина стрелки прокрутки

// Шаг прокрутки — одна кнопка с отступом
static constexpr int kScrollStep = kBtnW + kSpacing;

LayoutGalleryWidget::LayoutGalleryWidget(const QList<GridLayoutDef>& presets,
                                          QWidget* parent)
    : QWidget(parent)
{
    auto* outer = new QHBoxLayout(this);
    outer->setContentsMargins(0, 0, 0, 0);
    outer->setSpacing(2);


    m_leftArrow = new QPushButton("◀", this);
    m_leftArrow->setObjectName("galleryArrowBtn");
    m_leftArrow->setFixedSize(kArrowW, kBtnH);
    m_leftArrow->setFlat(true);
    m_leftArrow->setToolTip(tr("Прокрутить влево"));
    connect(m_leftArrow, &QPushButton::clicked, this, &LayoutGalleryWidget::onScrollLeft);


    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setObjectName("galleryScrollArea");
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setFrameShape(QFrame::NoFrame);


    m_scrollArea->setFixedSize(kBtnW * kVisibleCount + kSpacing * (kVisibleCount - 1), kBtnH);

    auto* container = new QWidget();
    container->setObjectName("galleryContainer");
    auto* containerLayout = new QHBoxLayout(container);
    containerLayout->setContentsMargins(0, 0, 0, 0);
    containerLayout->setSpacing(kSpacing);

    for (int i = 0; i < presets.size(); ++i) {
        auto* btn = new LayoutPreviewButton(presets[i], container);
        m_buttons.append(btn);
        containerLayout->addWidget(btn);

        connect(btn, &LayoutPreviewButton::clicked, this, [this, i] {
            setCurrentIndex(i);
            emit layoutSelected(i);
        });
    }

    containerLayout->addStretch();

    m_scrollArea->setWidget(container);


    connect(m_scrollArea->horizontalScrollBar(), &QScrollBar::valueChanged,
            this, &LayoutGalleryWidget::onScrollValueChanged);


    m_rightArrow = new QPushButton("▶", this);
    m_rightArrow->setObjectName("galleryArrowBtn");
    m_rightArrow->setFixedSize(kArrowW, kBtnH);
    m_rightArrow->setFlat(true);
    m_rightArrow->setToolTip(tr("Прокрутить вправо"));
    connect(m_rightArrow, &QPushButton::clicked, this, &LayoutGalleryWidget::onScrollRight);

    
    auto* sep = new QFrame(this);
    sep->setObjectName("ribbonGroupSep");
    sep->setFrameShape(QFrame::VLine);
    sep->setFrameShadow(QFrame::Plain);
    sep->setFixedWidth(1);


    m_configureBtn = new QPushButton(tr("Настроить"), this);
    m_configureBtn->setObjectName("galleryConfigBtn");
    m_configureBtn->setFixedHeight(kBtnH);
    m_configureBtn->setEnabled(false); 

    connect(m_configureBtn, &QPushButton::clicked,
            this, &LayoutGalleryWidget::configureRequested);

    outer->addWidget(m_leftArrow);
    outer->addWidget(m_scrollArea);
    outer->addWidget(m_rightArrow);
    outer->addSpacing(4);
    outer->addWidget(sep);
    outer->addSpacing(4);
    outer->addWidget(m_configureBtn);


    onScrollValueChanged();
}

void LayoutGalleryWidget::setCurrentIndex(int index)
{
    for (int i = 0; i < m_buttons.size(); ++i)
        m_buttons[i]->setChecked(i == index);
}

void LayoutGalleryWidget::onScrollLeft()
{
    auto* sb = m_scrollArea->horizontalScrollBar();

    const int cur  = sb->value();
    const int step = kScrollStep;
    const int next = ((cur - 1) / step) * step; 
    sb->setValue(qMax(next, sb->minimum()));
}

void LayoutGalleryWidget::onScrollRight()
{
    auto* sb = m_scrollArea->horizontalScrollBar();
    const int cur  = sb->value();
    const int step = kScrollStep;
    const int next = ((cur / step) + 1) * step;  
    sb->setValue(qMin(next, sb->maximum()));
}

void LayoutGalleryWidget::onScrollValueChanged()
{
    auto* sb = m_scrollArea->horizontalScrollBar();
    m_leftArrow->setEnabled(sb->value() > sb->minimum());
    m_rightArrow->setEnabled(sb->value() < sb->maximum());
}
