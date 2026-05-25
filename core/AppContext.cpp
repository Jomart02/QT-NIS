#include "AppContext.h"

#include "PluginManager.h"
#include "Logger.h"
#include "DirectoryManager.h"

AppContext& AppContext::instance() {
    static AppContext inst;
    return inst;
}

AppContext::AppContext() = default;
AppContext::~AppContext() = default;

PluginManager& AppContext::pluginManager() const {
    return PluginManager::instance();
}

Logger& AppContext::logger() const {
    return Logger::instance();
}

DirectoryManager& AppContext::dirs() const {
    return DirectoryManager::instance();
}
