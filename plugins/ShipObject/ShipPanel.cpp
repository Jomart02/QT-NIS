#include "ShipPanel.h"
#include "ShipObject.h"
#include "ShipObjectControls.h"

ShipPanel::ShipPanel(QWidget* parent)
    : ToolWidgetBase("ship_object", {ComponentWindow::ShowOnGrid}, parent)
{
    m_controls = new ShipObjectControls(this);
    layout()->addWidget(m_controls);
}

QWidget* ShipPanel::getWidget()
{
    auto* ship = new ShipObject();
    connect(m_controls, &ShipObjectControls::rotationChanged,
            ship,       &ShipObject::setOrientation);
    return ship;
}
