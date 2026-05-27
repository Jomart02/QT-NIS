#include "GridLayoutPlugin.h"
#include "LayoutGalleryWidget.h"
#include "AppContext.h"
#include "IUiHost.h"
#include "RibbonGroup.h"

// Предустановленные раскладки
QList<GridLayoutDef> GridLayoutPlugin::makePresets()
{
    //2 × 2 
    GridLayoutDef two2;
    two2.id          = "2x2";
    two2.displayName = "2 × 2";
    two2.rows        = 2;
    two2.cols        = 2;
    two2.cells       = {
        {0, 0, 1, 1}, {0, 1, 1, 1},
        {1, 0, 1, 1}, {1, 1, 1, 1},
    };

    // 1 сверху + 2 снизу 

    GridLayoutDef one2;
    one2.id          = "1+2";
    one2.displayName = "1 + 2";
    one2.rows        = 2;
    one2.cols        = 2;
    one2.cells       = {
        {0, 0, 1, 2},                    
        {1, 0, 1, 1}, {1, 1, 1, 1},   
    };

    //  2 сверху + 1 снизу
    GridLayoutDef two1;
    two1.id          = "2+1";
    two1.displayName = "2 + 1";
    two1.rows        = 2;
    two1.cols        = 2;
    two1.cells       = {
        {0, 0, 1, 1}, {0, 1, 1, 1},   
        {1, 0, 1, 2},                  
    };

    return {two2, one2, two1};
}

QFuture<void> GridLayoutPlugin::onAppEvent(AppEvent event)
{
    if (event != AppEvent::ReadyToShow)
        return makeReadyFuture();

    m_host = AppContext::instance().uiHost();
    if (!m_host)
        return makeReadyFuture();

    m_presets = makePresets();

    auto* group = new RibbonGroup(tr("Раскладки"));

    m_gallery = new LayoutGalleryWidget(m_presets, group);
    group->addWidget(m_gallery);

    connect(m_gallery, &LayoutGalleryWidget::layoutSelected,
            this, &GridLayoutPlugin::onLayoutSelected);

    m_gallery->setCurrentIndex(0);

    m_host->addRibbonGroup(tr("Главная"), group);
    return makeReadyFuture();
}

void GridLayoutPlugin::onLayoutSelected(int index)
{
    if (!m_host || index < 0 || index >= m_presets.size())
        return;
    m_host->applyGridLayout(m_presets[index]);
}
