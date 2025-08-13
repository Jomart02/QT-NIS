#include "NavigationInfoTool.h"
#include "ui_NavigationInfoTool.h"
#include "NavigationInfo.h"
#include "SettingsNavigation.h"
NavigationInfoTool::NavigationInfoTool(QWidget* parent): 
ToolWidgetBase(WindowsDef::WindowId::NAVI_INFO,{ComponentWindow::SettingsNet,ComponentWindow::ShowOnGrid} ,parent), 
ui(new Ui::NavigationInfoTool){
    setupUi(ui);
}


NavigationInfoTool::~NavigationInfoTool(){

}

QWidget* NavigationInfoTool::getWidget(){
     return new NavigationInfo();
}
BaseSettingsWindow* NavigationInfoTool::getSettingsWidget(){
    return new SettingsNavigation(QVariant());
}
