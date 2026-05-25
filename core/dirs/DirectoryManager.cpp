#include "DirectoryManager.h"

#include <QDir>
#include <QStandardPaths>

DirectoryManager& DirectoryManager::instance() {
    static DirectoryManager inst;
    return inst;
}

void DirectoryManager::init(const QString& appDir) {
    m_appDir = appDir;

    const QString base = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    m_configDir = base + "/config";
    m_logsDir   = base + "/logs";
    m_dataDir   = base + "/data";

    for (const QString& dir : {m_configDir, m_logsDir, m_dataDir})
        QDir().mkpath(dir);
}
