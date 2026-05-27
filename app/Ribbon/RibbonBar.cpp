#include "RibbonBar.h"
#include "RibbonGroup.h"
#include <QFrame>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QTabBar>
#include <QVBoxLayout>

RibbonBar::RibbonBar(QWidget* parent) : QWidget(parent)
{
    setObjectName("ribbonBar");

    auto* outer = new QVBoxLayout(this);
    outer->setContentsMargins(0, 0, 0, 0);
    outer->setSpacing(0);

    m_tabBar = new QTabBar(this);
    m_tabBar->setObjectName("ribbonTabBar");
    m_tabBar->setExpanding(false);
    m_tabBar->setDrawBase(false);

    m_stack = new QStackedWidget(this);
    m_stack->setObjectName("ribbonStack");

    outer->addWidget(m_tabBar);
    outer->addWidget(m_stack, 1);

    connect(m_tabBar, &QTabBar::currentChanged,
            this,     &RibbonBar::onTabChanged);
}

void RibbonBar::addGroup(const QString& tabTitle, RibbonGroup* group)
{
    if (!m_tabIndex.contains(tabTitle)) {
        m_tabBar->addTab(tabTitle);

        auto* page   = new QWidget(m_stack);
        auto* layout = new QHBoxLayout(page);
        layout->setContentsMargins(6, 0, 6, 0);
        layout->setSpacing(0);
        layout->addStretch();

        const int idx = m_stack->addWidget(page);
        m_tabIndex.insert(tabTitle, idx);
        m_pages.insert(idx, {page, layout});
    }

    const int     idx    = m_tabIndex[tabTitle];
    TabPage&      tp     = m_pages[idx];
    QHBoxLayout*  layout = tp.layout;

    if (layout->count() > 1) {
        auto* sep = new QFrame(tp.page);
        sep->setObjectName("ribbonGroupSep");
        sep->setFrameShape(QFrame::VLine);
        sep->setFrameShadow(QFrame::Plain);
        sep->setFixedWidth(1);
        layout->insertWidget(layout->count() - 1, sep);
    }

    group->setParent(tp.page);
    layout->insertWidget(layout->count() - 1, group);
}

// ---------------------------------------------------------------------------
void RibbonBar::onTabChanged(int index)
{
    m_stack->setCurrentIndex(index);
}
