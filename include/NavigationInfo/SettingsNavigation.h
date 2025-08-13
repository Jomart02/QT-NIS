#pragma once

#include "BaseSettingsWindow.h"

namespace Ui
{
    class SettingsNavigation;
}

class SettingsNavigation : public BaseSettingsWindow{
     Q_OBJECT
public:
     SettingsNavigation(QVariant settings, QWidget * parent = nullptr);
     ~SettingsNavigation();
protected:    
     virtual QVariant getParam() override; 
private:
     Ui::SettingsNavigation* ui;
};