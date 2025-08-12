#include "ShipObjectTool.h"
#include "ui_ShipObjectTool.h"
#include "ShipObject.h"

ShipObjectTool::ShipObjectTool(QWidget* parent): ToolWidgetBase(WindowsDef::WindowId::SHIP_MODEL, parent), ui(new Ui::ShipObjectTool){
    setupUi(ui);
}


ShipObjectTool::~ShipObjectTool(){

}

QWidget* ShipObjectTool::getWidget(){
     return new ShipObject();
}