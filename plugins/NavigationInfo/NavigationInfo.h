#pragma once

#include <QWidget>

namespace Ui
{
    class NavigationInfo;
}


class NavigationInfo : public QWidget{
     Q_OBJECT
public:
     NavigationInfo(QWidget *parent = nullptr);
     ~NavigationInfo();
private:
     Ui::NavigationInfo* ui;
};