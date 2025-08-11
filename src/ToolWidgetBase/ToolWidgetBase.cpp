#include "ToolWidgetBase.h"
#include <QHBoxLayout>
#include <QDebug>

ToolWidgetBase::ToolWidgetBase(WindowsDef::WindowId id, QWidget *parent) : QWidget(parent), m_id(id) {
    // Основной layout - вертикальный
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);

    // Панель для кнопок
    m_buttonPanel = new QWidget(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout(m_buttonPanel);
    buttonLayout->setContentsMargins(5, 5, 5, 0);
    buttonLayout->setSpacing(5);
 
     // Spacer для выравнивания кнопок вправо
    buttonLayout->addStretch();

    m_showModeButton = new QPushButton("✖", this);

    m_showModeButton->setFixedSize(24, 24);
 

    buttonLayout->addWidget(m_showModeButton);
 
    // Добавляем панель кнопок в основной layout
    m_mainLayout->addWidget(m_buttonPanel);
 
    // Оставляем место для контента (заполняется через setupUi)
    m_mainLayout->addStretch();

}

ToolWidgetBase::~ToolWidgetBase() {}