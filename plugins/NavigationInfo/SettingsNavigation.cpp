#include "SettingsNavigation.h"
#include "ui_SettingsNavigation.h"


SettingsNavigation::SettingsNavigation(QVariant settings, QWidget * parent ):BaseSettingsWindow(settings ,parent), 
ui(new Ui::SettingsNavigation){
     setupUi(ui);
     QString style = QString("QWidget#%1 { min-width: %2%; max-width: %2%; }").arg(metaObject()->className()).arg(400);
     setStyleSheet(style);
}
SettingsNavigation::~SettingsNavigation(){

}

QVariant SettingsNavigation::getParam(){
     return QVariant();
}  
