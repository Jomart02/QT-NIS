#include "MapViewPlugin.h"
#include "MapViewPanel.h"
#include "AppContext.h"
#include "IUiHost.h"

QFuture<void> MapViewPlugin::onAppEvent(AppEvent event) {
    if (event == AppEvent::ReadyToShow) {
        if (auto* host = AppContext::instance().uiHost())
            host->addToolPanel(id(), displayName(), new MapViewPanel());
    }
    return makeReadyFuture();
}
