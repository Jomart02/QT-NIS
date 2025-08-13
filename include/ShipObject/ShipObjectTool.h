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
signals:
     void setRotation(int x, int y, int z);
protected:
     virtual BaseSettingsWindow* getSettingsWidget() override;
private slots:
     void updateValue();
private:
     Ui::ShipObjectTool* ui;
};