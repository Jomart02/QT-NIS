#pragma once

#include <QList>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class ToolWidgetBase : public QWidget {
    Q_OBJECT
public:
    enum class ComponentWindow { ShowOnGrid, SettingsWindow };
    enum class RequestAdd      { Add, ClearAdd, Delete };
    enum class StatusWidget    { NoAdd = 0, AddProcess, Added };

    ToolWidgetBase(const QString& id,
                   QList<ComponentWindow> components,
                   QWidget* parent = nullptr);
    ~ToolWidgetBase() override;

    QString id() const { return m_id; }
    void setAddState(StatusWidget state);

    virtual QWidget* getWidget() { return nullptr; }

    virtual QWidget* getSettingsWidget() { return nullptr; }

signals:
    void showModeClicked(const QString& id, RequestAdd show, bool& accept);
    void settingsRequested(const QString& id, QWidget* settings);

protected:
    template<typename UiClass>
    void setupUi(UiClass* ui) {
        if (!m_contentWidget) {
            m_contentWidget = new QWidget(this);
            m_mainLayout->addWidget(m_contentWidget);
        }
        ui->setupUi(m_contentWidget);
    }

private slots:
    void clickShowButton();
    void clickSettingsButton();

private:
    void updateButton();

    QString                m_id;
    QList<ComponentWindow> m_components;
    QPushButton*           m_showModeButton = nullptr;
    QPushButton*           m_settingsBtn    = nullptr;
    QWidget*               m_contentWidget  = nullptr;
    QVBoxLayout*           m_mainLayout     = nullptr;
    StatusWidget           m_statusW        = StatusWidget::NoAdd;
};
