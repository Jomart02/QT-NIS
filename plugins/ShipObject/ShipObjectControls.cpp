#include "ShipObjectControls.h"
#include "ui_ShipObjectTool.h"

ShipObjectControls::ShipObjectControls(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::ShipObjectTool)
{
    ui->setupUi(this);
    connect(ui->horizontalSliderX, &QSlider::valueChanged, this, &ShipObjectControls::onSliderChanged);
    connect(ui->horizontalSliderY, &QSlider::valueChanged, this, &ShipObjectControls::onSliderChanged);
    connect(ui->horizontalSliderZ, &QSlider::valueChanged, this, &ShipObjectControls::onSliderChanged);
}

ShipObjectControls::~ShipObjectControls()
{
    delete ui;
}

void ShipObjectControls::onSliderChanged()
{
    emit rotationChanged(
        ui->horizontalSliderX->value(),
        ui->horizontalSliderY->value(),
        ui->horizontalSliderZ->value()
    );
}
