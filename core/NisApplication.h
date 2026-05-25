#pragma once

#include <QApplication>
#include "AppEvent.h"


class NisApplication : public QApplication {
    Q_OBJECT
public:
    NisApplication(int& argc, char** argv);

    static NisApplication* instance();
    void run();

signals:
    void eventDispatched(AppEvent event);

private:
    void dispatchEvent(AppEvent event);
};
