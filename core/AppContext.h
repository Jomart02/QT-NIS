#pragma once

class PluginManager;
class Logger;
class DirectoryManager;
class IUiHost;


class AppContext {
public:
    static AppContext& instance();

    PluginManager&    pluginManager() const;
    Logger&           logger()        const;
    DirectoryManager& dirs()          const;


    IUiHost* uiHost() const { return m_uiHost; }
    void     setUiHost(IUiHost* host) { m_uiHost = host; }

    AppContext(const AppContext&) = delete;
    AppContext& operator=(const AppContext&) = delete;

private:
    AppContext();
    ~AppContext();

    IUiHost* m_uiHost = nullptr;
};
