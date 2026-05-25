#include "NisApplication.h"

#include "AppContext.h"
#include "PluginManager.h"
#include "DirectoryManager.h"
#include "Logger.h"

NisApplication::NisApplication(int& argc, char** argv)
    : QApplication(argc, argv)
{
    Logger::install();
    DirectoryManager::instance().init(applicationDirPath());

    connect(this, &QApplication::aboutToQuit, this, [this]() {
        auto& pm = AppContext::instance().pluginManager();
        pm.dispatch(AppEvent::AboutToClose)
            .then(this, [this]() {
                AppContext::instance().pluginManager().dispatch(AppEvent::Closed);
            });
    });
}

NisApplication* NisApplication::instance() {
    return static_cast<NisApplication*>(QApplication::instance());
}

void NisApplication::run() {
    auto& pm = AppContext::instance().pluginManager();
    pm.loadAll(AppContext::instance().dirs().pluginsDir());

    pm.dispatch(AppEvent::PluginsLoaded)
        .then(this, [this]() {
            AppContext::instance().pluginManager().dispatch(AppEvent::ReadyToShow);
        });
}
