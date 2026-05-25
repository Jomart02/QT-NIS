#include "ToolWidgetBase.h"

#include <QHBoxLayout>
#include <QStyle>

ToolWidgetBase::ToolWidgetBase(const QString& id,
                               QList<ComponentWindow> components,
                               QWidget* parent)
    : QWidget(parent)
    , m_id(id)
    , m_components(std::move(components))
{
    setObjectName(id);

    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);

    const bool hasGrid     = m_components.contains(ComponentWindow::ShowOnGrid);
    const bool hasSettings = m_components.contains(ComponentWindow::SettingsWindow);

    if (hasGrid || hasSettings) {
        auto* btnLayout = new QHBoxLayout();
        btnLayout->setContentsMargins(5, 5, 5, 0);
        btnLayout->setSpacing(5);
        btnLayout->addStretch();

        if (hasGrid) {
            m_showModeButton = new QPushButton("✖", this);
            m_showModeButton->setProperty("showButton", true);
            m_showModeButton->setProperty("showStatus", 0);
            m_showModeButton->setFixedSize(24, 24);
            btnLayout->addWidget(m_showModeButton);
            connect(m_showModeButton, &QPushButton::clicked,
                    this, &ToolWidgetBase::clickShowButton);
        }

        if (hasSettings) {
            m_settingsBtn = new QPushButton("⚙", this);
            m_settingsBtn->setFixedSize(24, 24);
            m_settingsBtn->setCheckable(true);
            btnLayout->addWidget(m_settingsBtn);
            connect(m_settingsBtn, &QPushButton::clicked,
                    this, &ToolWidgetBase::clickSettingsButton);
        }

        m_mainLayout->addLayout(btnLayout);
    }
}

ToolWidgetBase::~ToolWidgetBase() = default;

void ToolWidgetBase::setAddState(StatusWidget state)
{
    m_statusW = state;
    updateButton();
}

void ToolWidgetBase::clickShowButton()
{
    bool accept = true;
    if (m_statusW == StatusWidget::NoAdd) {
        emit showModeClicked(m_id, RequestAdd::Add, accept);
        if (accept)
            m_statusW = StatusWidget::AddProcess;
    } else if (m_statusW == StatusWidget::AddProcess) {
        emit showModeClicked(m_id, RequestAdd::ClearAdd, accept);
        m_statusW = StatusWidget::NoAdd;
    } else if (m_statusW == StatusWidget::Added) {
        emit showModeClicked(m_id, RequestAdd::Delete, accept);
        m_statusW = StatusWidget::NoAdd;
    }
    updateButton();
}

void ToolWidgetBase::clickSettingsButton()
{
    if (QWidget* sw = getSettingsWidget())
        emit settingsRequested(m_id, sw);
}

void ToolWidgetBase::updateButton()
{
    if (!m_showModeButton) return;
    m_showModeButton->setProperty("showStatus", static_cast<int>(m_statusW));
    m_showModeButton->style()->unpolish(m_showModeButton);
    m_showModeButton->style()->polish(m_showModeButton);
    m_showModeButton->update();
}
