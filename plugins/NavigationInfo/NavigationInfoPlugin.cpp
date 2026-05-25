#include "NavigationInfoPlugin.h"
#include "NavigationInfoPanel.h"
#include "AppContext.h"
#include "IUiHost.h"
#include <qlogging.h>

QFuture<void> NavigationInfoPlugin::onAppEvent(AppEvent event) {
    if (event == AppEvent::ReadyToShow) {
        if (auto* host = AppContext::instance().uiHost()){
            host->addToolPanel(id(), displayName(), new NavigationInfoPanel());
        }
    }
    return makeReadyFuture();
}
