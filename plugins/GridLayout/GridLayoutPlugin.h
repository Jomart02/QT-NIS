#pragma once

#include "IPlugin.h"
#include "GridLayoutDef.h"

class IUiHost;
class LayoutGalleryWidget;


class GridLayoutPlugin : public IPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "nis.IPlugin/1.0" FILE "gridlayout.json")
    Q_INTERFACES(IPlugin)
public:
    QString id()          const override { return "grid_layout"; }
    QString displayName() const override { return tr("Grid Layout"); }

    QFuture<void> onAppEvent(AppEvent event) override;

private slots:
    // Применяет раскладку с заданным индексом.
    void onLayoutSelected(int index);

private:
    // Возвращает список предустановленных раскладок.
    static QList<GridLayoutDef> makePresets();

    IUiHost*             m_host    = nullptr;
    QList<GridLayoutDef> m_presets;
    LayoutGalleryWidget* m_gallery = nullptr; 
};
