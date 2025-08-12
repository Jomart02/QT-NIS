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
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
signals:
    void addState(WindowsDef::WindowId id,StateAdd state);
private slots:
    void showModeClicked(WindowsDef::WindowId id,ToolWidgetBase::RequestAdd show,bool &accept);
    void requestAdd(WindowsDef::WindowId selectAdd, QString nameCell);
    void removeWidgetFromGrid(WindowsDef::WindowId remove);
private:
    Ui::MainWindow* ui;
    ToolWidgetBase *currentAdd = nullptr;
};