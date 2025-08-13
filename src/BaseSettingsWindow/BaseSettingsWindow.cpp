#pragma once
#include "BaseSettingsWindow.h"
#include <QPushButton>
#include <QVBoxLayout>


BaseSettingsWindow::BaseSettingsWindow(QVariant settings,QWidget *parent ){
     m_mainLayout = new QVBoxLayout(this);
     m_mainLayout->setContentsMargins(0, 0, 0, 0);
     m_mainLayout->setSpacing(0);

     QHBoxLayout *buttonLayout = new QHBoxLayout();
     buttonLayout->setContentsMargins(5, 5, 5, 0);
     buttonLayout->setSpacing(5);
     m_mainLayout->addLayout(buttonLayout);
        
     m_saveButton = new QPushButton("Сохранить", this);
     buttonLayout->addWidget(m_saveButton);            
     connect(m_saveButton, &QPushButton::clicked,this, &BaseSettingsWindow::saveParamClicked);
}
BaseSettingsWindow::~BaseSettingsWindow(){

}

void BaseSettingsWindow::saveParamClicked(){

}
