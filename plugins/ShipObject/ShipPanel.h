#pragma once

#include "ToolWidgetBase.h"

class ShipObjectControls;

class ShipPanel : public ToolWidgetBase {
    Q_OBJECT
public:
    explicit ShipPanel(QWidget* parent = nullptr);
    QWidget* getWidget()         override;
    QWidget* getSettingsWidget() override { return nullptr; }

private:
    ShipObjectControls* m_controls = nullptr;
};
