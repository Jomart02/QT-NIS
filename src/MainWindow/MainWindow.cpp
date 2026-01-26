#include "globalVar.h"
#include "ui_MainWindow.h"
#include "MainWindow.h"
#include "ShipObjectTool.h"
#include <QFile>
#include <QTimer>
#include "NavigationInfoTool.h"
#include "MapViewTool.h"
#include "TopPanel.h"
#include <QScreen>
#include <QWindow>

MainWindow* MainWindow::_instance = nullptr;

MainWindow* MainWindow::getInstance(){
     if (_instance == nullptr) {
        _instance = new MainWindow();
    }
    return _instance;
}

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent), ui(new Ui::MainWindow),
topPanel(new TopPanel(this)){
    ui->setupUi(this);

    QFile file(":/styles/style");
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open QSS file:" ;
    }else{
        QString style = file.readAll();
        setStyleSheet(style);
    }

    ShipObjectTool *ship = new ShipObjectTool(this);
    NavigationInfoTool *navi = new NavigationInfoTool(this);
    MapViewTool *mapView =  new MapViewTool(this);

    ui->PanelWidget->addWidget("Ship", ship, ship->metaObject()->className());
    ui->PanelWidget->addWidget("Navi", navi, navi->metaObject()->className());
    ui->PanelWidget->addWidget("Map", mapView, mapView->metaObject()->className());

    connect(ship,&ToolWidgetBase::showModeClicked, this,&MainWindow::showModeClicked);
    connect(navi,&ToolWidgetBase::showModeClicked, this,&MainWindow::showModeClicked);
    connect(mapView,&ToolWidgetBase::showModeClicked, this,&MainWindow::showModeClicked);
    connect(this, &MainWindow::addState, ui->widgetGrid, &GridWidget::setAddState);
    connect(ui->widgetGrid,&GridWidget::addReady,this, &MainWindow::requestAdd);
    connect(ui->widgetGrid,&GridWidget::removeClicked,this, &MainWindow::removeWidgetFromGrid);

    ui->toolBar->addWidget(topPanel);

    connect(topPanel, &TopPanel::exitTriggered, [=](){
       this->deleteLater();
    });
    connect(topPanel, &TopPanel::changeVisRightPanel, this, &MainWindow::visibilityRightPanelChange);
}


MainWindow::~MainWindow(){

}

void MainWindow::initMultiDisplay() {
    QList<QScreen *> screens = QGuiApplication::screens();
    

    for (int i = 1; i < screens.size(); ++i) {
        GridWidget *extraGrid = new GridWidget(); 
        
        extraGrid->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
        extraGrid->show();
        
        extraGrid->windowHandle()->setScreen(screens[i]);
        extraGrid->showFullScreen();
        
        connect(this, &MainWindow::addState, extraGrid, &GridWidget::setAddState);
        connect(extraGrid,&GridWidget::addReady,this, &MainWindow::requestAdd);
        connect(extraGrid,&GridWidget::removeClicked,this, &MainWindow::removeWidgetFromGrid);
    }
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
            currentAdd = nullptr;
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
        QWidget *addWidget = child->getWidget();
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

void MainWindow::visibilityRightPanelChange(bool visible){
    ui->PanelWidget->setVisible(visible);
    QGridLayout *layout = qobject_cast<QGridLayout*>(centralWidget()->layout());
    if (layout) {
        if (visible) {
            layout->setColumnStretch(0, 1); 
            layout->setColumnStretch(1, 4); 
        } else {
            layout->setColumnStretch(0, 0); 
            layout->setColumnStretch(1, 5); 
        }
    }
}