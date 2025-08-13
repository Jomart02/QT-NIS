#include "ShipObjectTool.h"
#include "ui_ShipObjectTool.h"
#include "ShipObject.h"

ShipObjectTool::ShipObjectTool(QWidget* parent): ToolWidgetBase(WindowsDef::WindowId::SHIP_MODEL,{ComponentWindow::ShowOnGrid}, parent), ui(new Ui::ShipObjectTool){
    setupUi(ui);

    connect(ui->horizontalSliderX, &QSlider::valueChanged, this,&ShipObjectTool::updateValue);
    connect(ui->horizontalSliderY, &QSlider::valueChanged, this,&ShipObjectTool::updateValue);
    connect(ui->horizontalSliderZ, &QSlider::valueChanged, this,&ShipObjectTool::updateValue);
}


ShipObjectTool::~ShipObjectTool(){

}

QWidget* ShipObjectTool::getWidget(){
    ShipObject *ship = new ShipObject();
    connect(this,&ShipObjectTool::setRotation,ship,&ShipObject::setOrientation);
    return ship;
}

BaseSettingsWindow* ShipObjectTool::getSettingsWidget(){
    return nullptr;
}

void ShipObjectTool::updateValue(){
    int x = ui->horizontalSliderX->value();
    int y = ui->horizontalSliderY->value();
    int z = ui->horizontalSliderZ->value();

    emit setRotation(x,y,z);
}