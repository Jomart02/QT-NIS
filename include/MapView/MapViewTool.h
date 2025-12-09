#include "ToolWidgetBase.h"

namespace Ui
{
    class MapViewTool;
}
class MapViewTool : public ToolWidgetBase{
    Q_OBJECT
public:
    MapViewTool(QWidget *parent = nullptr);
    ~MapViewTool();
    virtual QWidget* getWidget() override;
protected:
    virtual BaseSettingsWindow* getSettingsWidget() override;
private:
    Ui::MapViewTool* ui;
};