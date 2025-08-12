
#pragma once

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "globalVar.h"

class ToolWidgetBase : public QWidget {
    Q_OBJECT

public:
    explicit ToolWidgetBase(WindowsDef::WindowId id, QWidget *parent = nullptr);
    ~ToolWidgetBase();
    WindowsDef::WindowId getId(){return m_id;}

    enum class RequestAdd{
        Add,
        ClearAdd,
        Delete
    };

    enum class StatusWidget{
        NoAdd,
        AddProcess,
        Added
    };
    void setAddState(StatusWidget state);
    virtual QWidget* getWidget() = 0;
signals:
    void showModeClicked(WindowsDef::WindowId id,RequestAdd show,bool &accept);
private slots:
    void clickShowButton();
protected:
    template<typename UiClass>
    void setupUi(UiClass *ui){
        if (!m_contentWidget) {
            m_contentWidget = new QWidget(this);
            m_mainLayout->insertWidget(1, m_contentWidget); 
        }
        ui->setupUi(m_contentWidget);
    }
private:
    WindowsDef::WindowId m_id;
    QPushButton *m_showModeButton;
    QWidget *m_buttonPanel;
    QWidget *m_contentWidget = nullptr;
    QVBoxLayout *m_mainLayout;
    StatusWidget m_statusW;
};