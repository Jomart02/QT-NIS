#pragma once
#include <QObject>
#include <qglobal.h>
#include <qvariant.h>

class BaseHandlerNaviData : public QObject{
    Q_OBJECT
public:
    BaseHandlerNaviData(QWidget *parent){}
    ~BaseHandlerNaviData(){}
signals:
    void newData(QVariant &data);
public slots:
    virtual void handlerNmea(QStringList &nmea) = 0;
};