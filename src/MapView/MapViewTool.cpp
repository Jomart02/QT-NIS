#include "MapViewTool.h"
#include "ui_MapViewTool.h"
#include "MapView.h"
MapViewTool::MapViewTool(QWidget* parent): 
ToolWidgetBase(WindowsDef::WindowId::MAP,{ComponentWindow::ShowOnGrid} ,parent), 
ui(new Ui::MapViewTool){
    setupUi(ui);
}


MapViewTool::~MapViewTool(){

}

QWidget* MapViewTool::getWidget(){
    return new MapView();
}
BaseSettingsWindow* MapViewTool::getSettingsWidget(){
    return nullptr;
}
