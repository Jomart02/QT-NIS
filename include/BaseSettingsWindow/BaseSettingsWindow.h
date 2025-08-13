#pragma once
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class BaseSettingsWindow : public QWidget{
     Q_OBJECT
public:
     BaseSettingsWindow(QVariant settings,QWidget *parent = nullptr);
     ~BaseSettingsWindow();
signals:
     void saveParam(QVariant &settings);
protected:
     template<typename UiClass>
     void setupUi(UiClass *ui){
          if (!m_contentWidget) {
               m_contentWidget = new QWidget(this);
               m_mainLayout->insertWidget(0, m_contentWidget); 
          }
          ui->setupUi(m_contentWidget);
     }
     virtual QVariant getParam() = 0;
private slots:
     void saveParamClicked();
private:
     QPushButton *m_saveButton = nullptr;
     QWidget *m_contentWidget = nullptr;
     QVBoxLayout *m_mainLayout = nullptr;
};