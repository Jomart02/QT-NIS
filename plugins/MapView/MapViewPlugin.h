#pragma once

#include "IPlugin.h"
#include "AppEvent.h"

class MapViewPlugin : public IPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "nis.IPlugin/1.0" FILE "mapview.json")
    Q_INTERFACES(IPlugin)
public:
    QString id()          const override { return "map_view"; }
    QString displayName() const override { return "Map View"; }

    QFuture<void> onAppEvent(AppEvent event) override;
};
