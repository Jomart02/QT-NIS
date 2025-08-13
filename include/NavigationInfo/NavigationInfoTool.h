#include "ToolWidgetBase.h"

namespace Ui
{
    class NavigationInfoTool;
}
class NavigationInfoTool : public ToolWidgetBase{
     Q_OBJECT
public:
     NavigationInfoTool(QWidget *parent = nullptr);
     ~NavigationInfoTool();
     virtual QWidget* getWidget() override;
protected:
     virtual BaseSettingsWindow* getSettingsWidget() override;
private:
     Ui::NavigationInfoTool* ui;
};