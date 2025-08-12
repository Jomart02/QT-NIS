#include "ToolWidgetBase.h"
#include <QHBoxLayout>
#include <QDebug>

ToolWidgetBase::ToolWidgetBase(WindowsDef::WindowId id, QWidget *parent) : QWidget(parent), m_id(id) {
    QMetaEnum metaEnum = QMetaEnum::fromType<WindowsDef::WindowId>();
    QString windowIdStr = metaEnum.valueToKey(static_cast<int>(m_id));
    setObjectName(windowIdStr);
    m_statusW = StatusWidget::NoAdd;
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
    m_showModeButton->setStyleSheet("QPushButton::checked{background-color:red;}");
    // Добавляем панель кнопок в основной layout
    m_mainLayout->addWidget(m_buttonPanel);
 
    // Оставляем место для контента (заполняется через setupUi)
    m_mainLayout->addStretch();
    connect(m_showModeButton, &QPushButton::clicked,this, &ToolWidgetBase::clickShowButton);
}

ToolWidgetBase::~ToolWidgetBase() {}

void ToolWidgetBase::setAddState(StatusWidget state){
    m_statusW = state;
    qDebug() << "AAAA";
    //Переключение отображения при добавленности 
}

void ToolWidgetBase::clickShowButton(){
    bool accept = true;
    if(m_statusW == StatusWidget::NoAdd){
        emit showModeClicked(m_id, RequestAdd::Add,accept);
        if(accept){
            // m_showModeButton->setChecked(false); //проаерти поменять и будет другая конопка
            m_statusW =StatusWidget::AddProcess;
        }
    }else if(m_statusW == StatusWidget::AddProcess){
        // m_showModeButton->setChecked(false); //проаерти поменять и будет другая конопка
        emit showModeClicked(m_id, RequestAdd::ClearAdd,accept);
        m_statusW = StatusWidget::NoAdd;
    }else if(m_statusW == StatusWidget::Added){
        bool accept = true;
        emit showModeClicked(m_id, RequestAdd::Delete,accept);
        m_statusW =StatusWidget::NoAdd;
    }
    
}