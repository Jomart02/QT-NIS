#include "globalVar.h"
#include "ui_MainWindow.h"
#include "MainWindow.h"
#include "VideoInput.h"

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    VideoInput *tool = new VideoInput(this);
    ui->PanelWidget->addWidget("Video",tool, "const QString &objectNamePage");
}


MainWindow::~MainWindow(){

}