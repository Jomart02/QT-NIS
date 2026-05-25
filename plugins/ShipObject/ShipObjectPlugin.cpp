#include "ShipObjectPlugin.h"
#include "ShipPanel.h"
#include "AppContext.h"
#include "IUiHost.h"

QFuture<void> ShipObjectPlugin::onAppEvent(AppEvent event) {
    if (event == AppEvent::ReadyToShow) {
        if (auto* host = AppContext::instance().uiHost())
            host->addToolPanel(id(), displayName(), new ShipPanel());
    }
    return makeReadyFuture();
}
