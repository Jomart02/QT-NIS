#pragma once

#include <QMainWindow>
#include <QMap>

#include "IUiHost.h"
#include "ToolWidgetBase.h"
#include "GridWidget.h"

namespace Ui { class MainWindow; }
class TopPanel;
class QAction;

class MainWindow : public QMainWindow, public IUiHost {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    // ── IUiHost ─────────────────────────────────────────────────────────────
    void addToolPanel(const QString& id, const QString& title,
                      ToolWidgetBase* panel)                               override;
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
    Ui::MainWindow*  ui;
    ToolWidgetBase*  m_currentAdd = nullptr;
    TopPanel*        m_topPanel   = nullptr;

    QMap<QString, QMenu*> m_menus;
};
