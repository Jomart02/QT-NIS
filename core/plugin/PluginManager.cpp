#include "PluginManager.h"

#include <QDir>
#include <QFuture>
#include <QPluginLoader>
#include <QPromise>
#include <QtConcurrent>
#include <QDebug>

PluginManager& PluginManager::instance() {
    static PluginManager inst;
    return inst;
}

PluginManager::PluginManager(QObject* parent)
    : QObject(parent)
{}

void PluginManager::loadAll(const QString& pluginsDir) {
    QDir dir(pluginsDir);
    if (!dir.exists()) {
        qWarning() << "[PluginManager] directory not found:" << pluginsDir;
        return;
    }

    const QStringList filters = {"*.so", "*.dll"};
    for (const QFileInfo& fi : dir.entryInfoList(filters, QDir::Files)) {
        const QString path = fi.absoluteFilePath();

        auto* loader = new QPluginLoader(path, this);
        QObject* obj = loader->instance();
        if (!obj) {
            qWarning() << "[PluginManager] failed to load" << path
                       << ":" << loader->errorString();
            delete loader;
            continue;
        }

        auto* plugin = qobject_cast<IPlugin*>(obj);
        if (!plugin) {
            qWarning() << "[PluginManager]" << path << "does not implement IPlugin";
            loader->unload();
            delete loader;
            continue;
        }

        m_loaders.append(loader);
        m_plugins.append(plugin);
        qInfo() << "[PluginManager] loaded plugin:" << plugin->id();
        emit pluginLoaded(plugin);
    }
}

QFuture<void> PluginManager::dispatch(AppEvent event) {
    if (m_plugins.isEmpty()) {
        QPromise<void> p;
        p.start();
        p.finish();
        return p.future();
    }

    QList<QFuture<void>> futures;
    futures.reserve(m_plugins.size());
    for (auto* plugin : m_plugins)
        futures.append(plugin->onAppEvent(event));

    return QtFuture::whenAll(futures.begin(), futures.end())
        .then(this, [this, event](QList<QFuture<void>>) {
            emit eventDispatched(event);
        });
}

QList<IPlugin*> PluginManager::plugins() const {
    return m_plugins;
}

IPlugin* PluginManager::pluginById(const QString& id) const {
    for (auto* p : m_plugins)
        if (p->id() == id) return p;
    return nullptr;
}
