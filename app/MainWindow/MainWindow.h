#pragma once

#include <QMainWindow>
#include <QMap>

#include "IUiHost.h"
#include "GridLayoutDef.h"
#include "ToolWidgetBase.h"
#include "GridWidget.h"

namespace Ui { class MainWindow; }
class RibbonBar;
class QAction;

class MainWindow : public QMainWindow, public IUiHost {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    // ── IUiHost ─────────────────────────────────────────────────────────────
    void addToolPanel(const QString& id, const QString& title,
                      ToolWidgetBase* panel)                               override;
    void addRibbonGroup(const QString& tabTitle,
                        RibbonGroup* group)                                override;
    void applyGridLayout(const GridLayoutDef& def)                         override;
    void showSettings(const QString& title, QWidget* settings)            override;
    void addStatusBarWidget(QWidget* widget, int stretch = 0)             override;
    void addMenuAction(const QString& menuTitle, QAction* action)         override;

signals:
    void addState(const QString& pluginId, StateAdd state);

private slots:
    void onShowModeClicked(const QString& pluginId, ToolWidgetBase::RequestAdd show, bool& accept);
    void onAddReady(const QString& pluginId, const QString& cellName);
    void onRemoveWidget(const QString& pluginId);
    void onVisibilityRightPanelChange(bool visible);

private:
    // Builds the built-in "Home" ribbon tab with system controls.
    void setupHomeRibbonTab();

    Ui::MainWindow* ui;
    ToolWidgetBase* m_currentAdd = nullptr;
    RibbonBar*      m_ribbon     = nullptr;

    QMap<QString, QMenu*> m_menus;
};
