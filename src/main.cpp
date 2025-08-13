#include <QApplication>
#include "MainWindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);   

    MainWindow *mainW = MainWindow::getInstance();
    mainW->showFullScreen();

    return app.exec(); 
}