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
private:
     Ui::NavigationInfoTool* ui;
};