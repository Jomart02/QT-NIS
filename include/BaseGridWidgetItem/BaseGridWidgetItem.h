#pragma once
#include <QWidget>

class BaseGridWidgetItem : public QWidget{
    Q_OBJECT
public:
    BaseGridWidgetItem(QWidget *parent){}
    ~BaseGridWidgetItem(){}
public slots:
    virtual void handlerNewData(QVariant &data) = 0;
};