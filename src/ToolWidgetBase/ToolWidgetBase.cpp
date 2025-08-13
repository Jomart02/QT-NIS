#include "ToolWidgetBase.h"
#include <QHBoxLayout>
#include <QDebug>
#include <QStyle>
#include "MainWindow.h"
#include "DockContainer.h"

ToolWidgetBase::ToolWidgetBase(WindowsDef::WindowId id, QWidget *parent) : QWidget(parent), m_id(id) {
    QMetaEnum metaEnum = QMetaEnum::fromType<WindowsDef::WindowId>();
    QString windowIdStr = metaEnum.valueToKey(static_cast<int>(m_id));
    setObjectName(windowIdStr);
    m_statusW = StatusWidget::NoAdd;
    // Основной layout - вертикальный
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);
}

ToolWidgetBase::ToolWidgetBase(WindowsDef::WindowId id,QList<ComponentWindow> m_components, QWidget *parent):ToolWidgetBase(id,parent){

    if(!m_components.empty()){
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        buttonLayout->setContentsMargins(5, 5, 5, 0);
        buttonLayout->setSpacing(5);
    
        // Spacer для выравнивания кнопок вправо
        buttonLayout->addStretch();
        m_mainLayout->addLayout(buttonLayout);
        if(m_components.contains(ComponentWindow::ShowOnGrid)){
            m_showModeButton = new QPushButton("✖", this);
            m_showModeButton->setProperty("showButton", true);
            m_showModeButton->setProperty("showStatus", 0);
            m_showModeButton->setFixedSize(24, 24);
        
            buttonLayout->addWidget(m_showModeButton);            
            connect(m_showModeButton, &QPushButton::clicked,this, &ToolWidgetBase::clickShowButton);
        }
        if(m_components.contains(ComponentWindow::SettingsNet)){
            m_settingsNet = new QPushButton("⚙️", this);
            m_settingsNet->setFixedSize(24, 24);
            m_settingsNet->setCheckable(true);
            buttonLayout->addWidget(m_settingsNet);
            connect(m_settingsNet, &QPushButton::clicked,this, &ToolWidgetBase::clickSettingsButton);
        }
    }
}

ToolWidgetBase::~ToolWidgetBase() {}

void ToolWidgetBase::setAddState(StatusWidget state){
    m_statusW = state;
    updateButton();
}

void ToolWidgetBase::clickShowButton(){
    bool accept = true;
    if(m_statusW == StatusWidget::NoAdd){
        emit showModeClicked(m_id, RequestAdd::Add,accept);
        if(accept){
            m_statusW =StatusWidget::AddProcess;
        }
    }else if(m_statusW == StatusWidget::AddProcess){
        emit showModeClicked(m_id, RequestAdd::ClearAdd,accept);
        m_statusW = StatusWidget::NoAdd;
    }else if(m_statusW == StatusWidget::Added){
        emit showModeClicked(m_id, RequestAdd::Delete,accept);
        m_statusW =StatusWidget::NoAdd;
    }
    updateButton();

}

void ToolWidgetBase::clickSettingsButton(bool ckecked){
    BaseSettingsWindow *set = getSettingsWidget();
    DockContainer *dock = new DockContainer("DDD", set,MainWindow::getInstance());
    dock->move(
        MainWindow::getInstance()->width() / 2 - dock->width() / 2,
        MainWindow::getInstance()->height() / 2 - dock->height() / 2
    );
    dock->show();
}

void ToolWidgetBase::updateButton(){
    if(!m_showModeButton) return;
    m_showModeButton->setProperty("showStatus", (int)m_statusW);

    m_showModeButton->style()->unpolish(m_showModeButton);
    m_showModeButton->style()->polish(m_showModeButton);
    m_showModeButton->update();
}
