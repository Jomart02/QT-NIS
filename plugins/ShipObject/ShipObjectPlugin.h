#pragma once

#include "IPlugin.h"
#include "AppEvent.h"

class ShipObjectPlugin : public IPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "nis.IPlugin/1.0" FILE "shipobject.json")
    Q_INTERFACES(IPlugin)
public:
    QString id()          const override { return "ship_object"; }
    QString displayName() const override { return "Ship 3D"; }

    QFuture<void> onAppEvent(AppEvent event) override;
};
