#pragma once

#include <QString>


class DirectoryManager {
public:
    static DirectoryManager& instance();

    void init(const QString& appDir);

    QString appDir()     const { return m_appDir; }
    QString pluginsDir() const { return m_appDir; }
    QString configDir()  const { return m_configDir; }
    QString logsDir()    const { return m_logsDir; }
    QString dataDir()    const { return m_dataDir; }

    DirectoryManager(const DirectoryManager&) = delete;
    DirectoryManager& operator=(const DirectoryManager&) = delete;

private:
    DirectoryManager() = default;

    QString m_appDir;
    QString m_configDir;
    QString m_logsDir;
    QString m_dataDir;
};
