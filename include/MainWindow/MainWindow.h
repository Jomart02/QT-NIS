#pragma once

#include <QMainWindow>
#include "globalVar.h"
#include "ToolWidgetBase.h"
namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static MainWindow* getInstance();
    
    ~MainWindow();
signals:
    void addState(WindowsDef::WindowId id,StateAdd state);
private slots:
    void showModeClicked(WindowsDef::WindowId id,ToolWidgetBase::RequestAdd show,bool &accept);
    void requestAdd(WindowsDef::WindowId selectAdd, QString nameCell);
    void removeWidgetFromGrid(WindowsDef::WindowId remove);
private:
    MainWindow(QWidget* parent = nullptr);
private:
    
    Ui::MainWindow* ui;
    ToolWidgetBase *currentAdd = nullptr;
    static MainWindow* _instance; 
};