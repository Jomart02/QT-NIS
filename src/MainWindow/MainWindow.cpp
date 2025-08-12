#include "globalVar.h"
#include "ui_MainWindow.h"
#include "MainWindow.h"
#include "ShipObjectTool.h"
#include <QFile>
#include <QTimer>
MainWindow::MainWindow(QWidget* parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    QFile file(":/styles/style");
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open QSS file:" ;
    }else{
        QString style = file.readAll();
        setStyleSheet(style);
    }

    ShipObjectTool *ship = new ShipObjectTool(this);

    ui->PanelWidget->addWidget("Ship", ship, ship->metaObject()->className());
    connect(ship,&ToolWidgetBase::showModeClicked, this,&MainWindow::showModeClicked);
    connect(this, &MainWindow::addState, ui->widgetGrid, &GridWidget::setAddState);
    connect(ui->widgetGrid,&GridWidget::addReady,this, &MainWindow::requestAdd);
    connect(ui->widgetGrid,&GridWidget::removeClicked,this, &MainWindow::removeWidgetFromGrid);

}


MainWindow::~MainWindow(){

}

void MainWindow::showModeClicked(WindowsDef::WindowId id,ToolWidgetBase::RequestAdd show,bool &accept){
    
    ToolWidgetBase *senderAdd = qobject_cast<ToolWidgetBase*>(sender());
    if(senderAdd){
        if(show == ToolWidgetBase::RequestAdd::Add){
            if(currentAdd) {
                accept = false;
                return;
            }
            currentAdd = senderAdd;
            emit addState(id, StateAdd::ReadyAdd);
        }else if(show == ToolWidgetBase::RequestAdd::ClearAdd){
            currentAdd = nullptr;
            emit addState(WindowsDef::WindowId::UNDEFINED,StateAdd::Normal );
        }else if(show == ToolWidgetBase::RequestAdd::Delete){
            ui->widgetGrid->removeWidget(id);
        }
    }else{
        accept = false;
    }
}

void MainWindow::requestAdd(WindowsDef::WindowId selectAdd, QString nameCell){
    QMetaEnum metaEnum = QMetaEnum::fromType<WindowsDef::WindowId>();
    QString windowIdStr = metaEnum.valueToKey(static_cast<int>(selectAdd));
    ToolWidgetBase *child = findChild<ToolWidgetBase*>(windowIdStr);
    if(child){
        QWidget *addWidget =  child->getWidget();
        addWidget->setParent(this);
        ui->widgetGrid->addWidgetToCell(nameCell, addWidget);
        currentAdd->setAddState(ToolWidgetBase::StatusWidget::Added);
        currentAdd = nullptr;
        if(selectAdd == WindowsDef::WindowId::SHIP_MODEL){
            QTimer::singleShot(200,[=](){ showFullScreen();});
        }
    }
}


void MainWindow::removeWidgetFromGrid(WindowsDef::WindowId remove){
    QMetaEnum metaEnum = QMetaEnum::fromType<WindowsDef::WindowId>();
    QString windowIdStr = metaEnum.valueToKey(static_cast<int>(remove));
    ToolWidgetBase *child = findChild<ToolWidgetBase*>(windowIdStr);
    if(child){
        child->setAddState(ToolWidgetBase::StatusWidget::NoAdd);
    }
}