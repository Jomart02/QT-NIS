#include <QApplication>
#include "MainWindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);   

    MainWindow *mainW = new MainWindow();
    mainW->show();

    return app.exec(); 
}