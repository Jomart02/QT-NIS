#pragma once

#include <QString>

class QWidget;
class QAction;
class ToolWidgetBase;


class IUiHost {
public:
    virtual ~IUiHost() = default;

    virtual void addToolPanel(const QString& id,
                              const QString& title,
                              ToolWidgetBase* panel) = 0;

    virtual void showSettings(const QString& title, QWidget* settings) = 0;
    virtual void addStatusBarWidget(QWidget* widget, int stretch = 0) = 0;
    virtual void addMenuAction(const QString& menuTitle, QAction* action) = 0;
};
