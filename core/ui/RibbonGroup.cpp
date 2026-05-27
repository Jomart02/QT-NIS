#include "RibbonGroup.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QVBoxLayout>

RibbonGroup::RibbonGroup(const QString& title, QWidget* parent)
    : QWidget(parent)
    , m_title(title)
{
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

    auto* outer = new QVBoxLayout(this);
    outer->setContentsMargins(4, 4, 4, 0);
    outer->setSpacing(0);

    auto* contentWidget = new QWidget(this);
    contentWidget->setObjectName("ribbonGroupContent");
    contentWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

    m_contentLayout = new QHBoxLayout(contentWidget);
    m_contentLayout->setContentsMargins(4, 4, 4, 4);
    m_contentLayout->setSpacing(6);
  
    auto* divider = new QFrame(this);
    divider->setObjectName("ribbonGroupDivider");
    divider->setFrameShape(QFrame::HLine);
    divider->setFrameShadow(QFrame::Plain);
    divider->setFixedHeight(1);

    auto* titleLabel = new QLabel(title, this);
    titleLabel->setObjectName("ribbonGroupTitle");
    titleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    titleLabel->setFixedHeight(18);

    outer->addWidget(contentWidget, 1);
    outer->addWidget(divider);
    outer->addWidget(titleLabel);
}

void RibbonGroup::addWidget(QWidget* widget)
{
    m_contentLayout->addWidget(widget);
}

void RibbonGroup::addSeparator()
{
    auto* sep = new QFrame(this);
    sep->setObjectName("ribbonGroupSep");
    sep->setFrameShape(QFrame::VLine);
    sep->setFrameShadow(QFrame::Plain);
    sep->setFixedWidth(1);
    m_contentLayout->addWidget(sep);
}
