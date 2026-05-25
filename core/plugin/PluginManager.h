#pragma once

#include <QFuture>
#include <QObject>
#include <QList>

#include "AppEvent.h"
#include "IPlugin.h"

class QPluginLoader;

class PluginManager : public QObject {
    Q_OBJECT
public:
    static PluginManager& instance();

    void loadAll(const QString& pluginsDir);

    QFuture<void> dispatch(AppEvent event);

    QList<IPlugin*> plugins()                    const;
    IPlugin*        pluginById(const QString& id) const;

signals:
    void pluginLoaded(IPlugin* plugin);
    void eventDispatched(AppEvent event);

private:
    explicit PluginManager(QObject* parent = nullptr);

    QList<QPluginLoader*> m_loaders;
    QList<IPlugin*>       m_plugins;
};
