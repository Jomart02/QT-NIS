#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "TopPanel.h"
#include "DockContainer.h"
#include "Logger.h"

#include <QAction>
#include <QFile>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QGridLayout>
#include <qlogging.h>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_topPanel(new TopPanel(this))
{
    ui->setupUi(this);

    QFile file(":/styles/style");
    if (file.open(QIODevice::ReadOnly))
        setStyleSheet(file.readAll());
    else
        qWarning() << "Could not open QSS file";

    connect(this,           &MainWindow::addState,          ui->widgetGrid, &GridWidget::setAddState);
    connect(ui->widgetGrid, &GridWidget::addReady,          this,           &MainWindow::onAddReady);
    connect(ui->widgetGrid, &GridWidget::removeClicked,     this,           &MainWindow::onRemoveWidget);
    connect(m_topPanel,     &TopPanel::exitTriggered,       [this]{ deleteLater(); });
    connect(m_topPanel,     &TopPanel::changeVisRightPanel, this, &MainWindow::onVisibilityRightPanelChange);

    ui->toolBar->addWidget(m_topPanel);
}

MainWindow::~MainWindow() = default;

void MainWindow::addToolPanel(const QString& id, const QString& title, ToolWidgetBase* panel)
{
    panel->setParent(this);
   
    connect(panel, &ToolWidgetBase::showModeClicked,
            this,  &MainWindow::onShowModeClicked);
    connect(panel, &ToolWidgetBase::settingsRequested,
            this,  [this](const QString& /*id*/, QWidget* sw){ showSettings("Settings", sw); });
    
    ui->PanelWidget->addWidget(title, panel, id);
    qInfo() << "[MainWindow] registered plugin panel:" << id;
}

void MainWindow::showSettings(const QString& title, QWidget* settings)
{
    auto* dock = new DockContainer(title, settings, this);
    dock->move(width() / 2 - dock->width() / 2,
               height() / 2 - dock->height() / 2);
    dock->show();
}

void MainWindow::addStatusBarWidget(QWidget* widget, int stretch) {
    statusBar()->addPermanentWidget(widget, stretch);
}

void MainWindow::addMenuAction(const QString& menuTitle, QAction* action) {
    if (!m_menus.contains(menuTitle)) {
        auto* menu = menuBar()->addMenu(menuTitle);
        m_menus.insert(menuTitle, menu);
    }
    m_menus[menuTitle]->addAction(action);
}

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

void MainWindow::onAddReady(const QString& pluginId, const QString& cellName){
    auto* panel = findChild<ToolWidgetBase*>(pluginId);
    if (!panel || !m_currentAdd) return;

    if (QWidget* content = panel->getWidget())
        ui->widgetGrid->addWidgetToCell(cellName, content);

    m_currentAdd->setAddState(ToolWidgetBase::StatusWidget::Added);
    m_currentAdd = nullptr;
}

void MainWindow::onRemoveWidget(const QString& pluginId) {
    if (auto* panel = findChild<ToolWidgetBase*>(pluginId))
        panel->setAddState(ToolWidgetBase::StatusWidget::NoAdd);
}

void MainWindow::onVisibilityRightPanelChange(bool visible) {
    ui->PanelWidget->setVisible(visible);
    auto* layout = qobject_cast<QGridLayout*>(centralWidget()->layout());
    if (layout) {
        layout->setColumnStretch(0, visible ? 1 : 0);
        layout->setColumnStretch(1, visible ? 4 : 5);
    }
}
