#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "DockContainer.h"
#include "RibbonBar.h"
#include "RibbonGroup.h"

#include <QAction>
#include <QFile>
#include <QGridLayout>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QStatusBar>
#include <qlogging.h>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_ribbon(new RibbonBar(this))
{
    ui->setupUi(this);

    QFile file(":/styles/style");
    if (file.open(QIODevice::ReadOnly))
        setStyleSheet(file.readAll());
    else
        qWarning() << "Could not open QSS file";

    // ── Wire grid signals ────────────────────────────────────────────────────
    connect(this,           &MainWindow::addState,      ui->widgetGrid, &GridWidget::setAddState);
    connect(ui->widgetGrid, &GridWidget::addReady,      this,           &MainWindow::onAddReady);
    connect(ui->widgetGrid, &GridWidget::removeClicked, this,           &MainWindow::onRemoveWidget);

    // ── Embed the ribbon in the toolbar ─────────────────────────────────────
    ui->toolBar->addWidget(m_ribbon);

    // ── Built-in system tab ──────────────────────────────────────────────────
    setupHomeRibbonTab();
}

MainWindow::~MainWindow() = default;

// ---------------------------------------------------------------------------
// Private helpers
// ---------------------------------------------------------------------------

void MainWindow::setupHomeRibbonTab()
{
    auto* appGroup = new RibbonGroup(tr("Application"), this);

    auto* exitBtn = new QPushButton(tr("Exit"), appGroup);
    exitBtn->setObjectName("ribbonExitBtn");
    connect(exitBtn, &QPushButton::clicked, this, &QWidget::close);
    appGroup->addWidget(exitBtn);

    auto* panelBtn = new QPushButton(tr("Side Panel"), appGroup);
    panelBtn->setObjectName("ribbonPanelBtn");
    panelBtn->setCheckable(true);
    panelBtn->setChecked(true);
    connect(panelBtn, &QPushButton::toggled,
            this,     &MainWindow::onVisibilityRightPanelChange);
    appGroup->addWidget(panelBtn);

    addRibbonGroup(tr("Главная"), appGroup);
}

// ---------------------------------------------------------------------------
// IUiHost
// ---------------------------------------------------------------------------

void MainWindow::addToolPanel(const QString& id, const QString& title,
                               ToolWidgetBase* panel)
{
    panel->setParent(this);

    connect(panel, &ToolWidgetBase::showModeClicked,
            this,  &MainWindow::onShowModeClicked);
    connect(panel, &ToolWidgetBase::settingsRequested,
            this,  [this](const QString& /*id*/, QWidget* sw){
                showSettings(tr("Settings"), sw);
            });

    ui->PanelWidget->addWidget(title, panel, id);
    qInfo() << "[MainWindow] registered plugin panel:" << id;
}

void MainWindow::addRibbonGroup(const QString& tabTitle, RibbonGroup* group)
{
    m_ribbon->addGroup(tabTitle, group);
}

void MainWindow::applyGridLayout(const GridLayoutDef& def)
{
    // Если в данный момент пользователь выбирает ячейку для плагина —
    // сбрасываем это состояние перед перестройкой сетки.
    if (m_currentAdd) {
        m_currentAdd->setAddState(ToolWidgetBase::StatusWidget::NoAdd);
        m_currentAdd = nullptr;
        emit addState(QString{}, StateAdd::Normal);
    }
    ui->widgetGrid->applyLayout(def);
}

void MainWindow::showSettings(const QString& title, QWidget* settings)
{
    auto* dock = new DockContainer(title, settings, this);
    dock->move(width()  / 2 - dock->width()  / 2,
               height() / 2 - dock->height() / 2);
    dock->show();
}

void MainWindow::addStatusBarWidget(QWidget* widget, int stretch)
{
    statusBar()->addPermanentWidget(widget, stretch);
}

void MainWindow::addMenuAction(const QString& menuTitle, QAction* action)
{
    if (!m_menus.contains(menuTitle)) {
        auto* menu = menuBar()->addMenu(menuTitle);
        m_menus.insert(menuTitle, menu);
    }
    m_menus[menuTitle]->addAction(action);
}

// ---------------------------------------------------------------------------
// Private slots
// ---------------------------------------------------------------------------

void MainWindow::onShowModeClicked(const QString& pluginId,
                                    ToolWidgetBase::RequestAdd show,
                                    bool& accept)
{
    auto* senderPanel = qobject_cast<ToolWidgetBase*>(sender());
    if (!senderPanel) { accept = false; return; }

    if (show == ToolWidgetBase::RequestAdd::Add) {
        if (m_currentAdd) { accept = false; return; }
        m_currentAdd = senderPanel;
        emit addState(pluginId, StateAdd::ReadyAdd);

    } else if (show == ToolWidgetBase::RequestAdd::ClearAdd) {
        m_currentAdd = nullptr;
        emit addState(QString{}, StateAdd::Normal);

    } else if (show == ToolWidgetBase::RequestAdd::Delete) {
        ui->widgetGrid->removeWidget(pluginId);
        m_currentAdd = nullptr;
    }
    accept = true;
}

void MainWindow::onAddReady(const QString& pluginId, const QString& cellName)
{
    auto* panel = findChild<ToolWidgetBase*>(pluginId);
    if (!panel || !m_currentAdd) return;

    if (QWidget* content = panel->getWidget())
        ui->widgetGrid->addWidgetToCell(cellName, content);

    m_currentAdd->setAddState(ToolWidgetBase::StatusWidget::Added);
    m_currentAdd = nullptr;
}

void MainWindow::onRemoveWidget(const QString& pluginId)
{
    if (auto* panel = findChild<ToolWidgetBase*>(pluginId))
        panel->setAddState(ToolWidgetBase::StatusWidget::NoAdd);
}

void MainWindow::onVisibilityRightPanelChange(bool visible)
{
    ui->PanelWidget->setVisible(visible);

    // The central widget has a QGridLayout with columns [PanelWidget, GridWidget].
    // Adjust stretch so GridWidget expands when the panel is hidden.
    if (auto* layout = qobject_cast<QGridLayout*>(centralWidget()->layout())) {
        layout->setColumnStretch(0, visible ? 1 : 0);
        layout->setColumnStretch(1, visible ? 4 : 5);
    }
}
