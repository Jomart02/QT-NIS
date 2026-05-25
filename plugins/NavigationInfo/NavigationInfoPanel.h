#pragma once

#include "ToolWidgetBase.h"

class NavigationInfoPanel : public ToolWidgetBase {
    Q_OBJECT
public:
    explicit NavigationInfoPanel(QWidget* parent = nullptr)
        : ToolWidgetBase("navigation_info",
                         {ComponentWindow::ShowOnGrid, ComponentWindow::SettingsWindow},
                         parent) {}

    QWidget* getWidget()         override;
    QWidget* getSettingsWidget() override;
};
