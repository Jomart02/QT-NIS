
#pragma once

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <qlist.h>
#include "globalVar.h"


class ToolWidgetBase : public QWidget {
    Q_OBJECT
public:
    enum class ComponentWindow{
        ShowOnGrid,
        SettingsNet
    };
    
    ToolWidgetBase(WindowsDef::WindowId id, QWidget *parent = nullptr);
    ToolWidgetBase(WindowsDef::WindowId id,QList<ComponentWindow> m_components, QWidget *parent = nullptr);
    ~ToolWidgetBase();
    WindowsDef::WindowId getId(){return m_id;}

    enum class RequestAdd{
        Add,
        ClearAdd,
        Delete
    };

    enum class StatusWidget{
        NoAdd = 0,
        AddProcess,
        Added
    };



    void setAddState(StatusWidget state);
    virtual QWidget* getWidget() = 0;
signals:
    void showModeClicked(WindowsDef::WindowId id,RequestAdd show,bool &accept);
private slots:
    void clickShowButton();
    void clickSettingsButton(bool ckecked);
protected:
    template<typename UiClass>
    void setupUi(UiClass *ui){
        if (!m_contentWidget) {
            m_contentWidget = new QWidget(this);
            m_mainLayout->insertWidget(m_mainLayout->count(), m_contentWidget); 
        }
        ui->setupUi(m_contentWidget);
    }
private:
    void updateButton();
private:
    WindowsDef::WindowId m_id;
    QPushButton *m_showModeButton = nullptr;
    QPushButton *m_settingsNet = nullptr;
    QWidget *m_buttonPanel = nullptr;
    QWidget *m_contentWidget = nullptr;
    QVBoxLayout *m_mainLayout = nullptr;
    StatusWidget m_statusW;
    QList<ComponentWindow> m_components;
};