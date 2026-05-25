#pragma once

#include "IPlugin.h"
#include "AppEvent.h"

class NavigationInfoPlugin : public IPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "nis.IPlugin/1.0" FILE "navigationinfo.json")
    Q_INTERFACES(IPlugin)
public:
    QString id()          const override { return "navigation_info"; }
    QString displayName() const override { return "Navigation Info"; }

    QFuture<void> onAppEvent(AppEvent event) override;
};
