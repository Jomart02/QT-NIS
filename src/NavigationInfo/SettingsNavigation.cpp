#pragma once

#include "SettingsNavigation.h"
#include "ui_SettingsNavigation.h"


SettingsNavigation::SettingsNavigation(QVariant settings, QWidget * parent ):BaseSettingsWindow(settings ,parent), 
ui(new Ui::SettingsNavigation){
     setupUi(ui);
}
SettingsNavigation::~SettingsNavigation(){

}

QVariant SettingsNavigation::getParam(){
     return QVariant();
}  
