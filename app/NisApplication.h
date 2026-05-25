#pragma once

#include <QApplication>
#include "AppEvent.h"

class MainWindow;

class NisApplication : public QApplication {
    Q_OBJECT
public:
    NisApplication(int& argc, char** argv);

    static NisApplication* instance();

    void run();

private:
    MainWindow* m_mainWindow = nullptr;
};
