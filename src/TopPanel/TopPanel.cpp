#include "TopPanel.h"
#include "ui_TopPanel.h"

TopPanel::TopPanel(QWidget *parent) : QWidget(parent) , ui(new Ui::TopPanel){
     ui->setupUi(this);
     connect(ui->pushButton_Exit, &QPushButton::clicked,this,&TopPanel::exitTriggered);
     connect(ui->pushButton_rightPanel, &QPushButton::clicked,this,&TopPanel::changeVisRightPanel);
}   
TopPanel::~TopPanel()
{
    delete ui;
}
