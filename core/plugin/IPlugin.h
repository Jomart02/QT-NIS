#pragma once

#include <QFuture>
#include <QObject>
#include <QIcon>
#include <QPromise>

#include "AppEvent.h"

class IPlugin : public QObject {
    Q_OBJECT
public:
    ~IPlugin() override = default;

    virtual QString id()          const = 0;
    virtual QString displayName() const = 0;
    virtual QIcon   icon()        const { return {}; }

    virtual QFuture<void> onAppEvent(AppEvent event) {
        Q_UNUSED(event)
        return makeReadyFuture();
    }

protected:
    static QFuture<void> makeReadyFuture() {
        QPromise<void> p;
        p.start();
        p.finish();
        return p.future();
    }
};

Q_DECLARE_INTERFACE(IPlugin, "nis.IPlugin/1.0")
