#pragma once

#include "ToolWidgetBase.h"

class MapViewPanel : public ToolWidgetBase {
    Q_OBJECT
public:
    explicit MapViewPanel(QWidget* parent = nullptr)
        : ToolWidgetBase("map_view", {ComponentWindow::ShowOnGrid}, parent) {}

    QWidget* getWidget() override;
};
