#include "NavigationInfoPanel.h"
#include "NavigationInfo.h"
#include "SettingsNavigation.h"

QWidget* NavigationInfoPanel::getWidget()
{
    return new NavigationInfo();
}

QWidget* NavigationInfoPanel::getSettingsWidget()
{
    return new SettingsNavigation(QVariant());
}
