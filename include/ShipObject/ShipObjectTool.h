#include "ToolWidgetBase.h"

namespace Ui
{
    class ShipObjectTool;
}


class ShipObjectTool : public ToolWidgetBase{
     Q_OBJECT
public:
     ShipObjectTool(QWidget *parent = nullptr);
     ~ShipObjectTool();
     virtual QWidget* getWidget() override;
private:
     Ui::ShipObjectTool* ui;
};