#include "NisApplication.h"

#include "MainWindow.h"
#include "AppContext.h"
#include "PluginManager.h"
#include "DirectoryManager.h"
#include "Logger.h"
#include "NisStyle.h"
#include "NisPalette.h"

NisApplication::NisApplication(int& argc, char** argv)
    : QApplication(argc, argv)
{
    setStyle(new NisStyle());
    setPalette(NisPalette::dark());

    Logger::install();
    DirectoryManager::instance().init(applicationDirPath());

    m_mainWindow = new MainWindow();
    AppContext::instance().setUiHost(m_mainWindow);

    connect(&AppContext::instance().pluginManager(), &PluginManager::eventDispatched,
            m_mainWindow, [this](AppEvent event) {
                if (event == AppEvent::ReadyToShow) {
                    m_mainWindow->showFullScreen();
                    AppContext::instance().pluginManager().dispatch(AppEvent::Shown);
                }
            });

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
