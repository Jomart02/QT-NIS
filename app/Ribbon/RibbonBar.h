#pragma once

#include <QMap>
#include <QString>
#include <QWidget>

class RibbonGroup;
class QHBoxLayout;
class QStackedWidget;
class QTabBar;


class RibbonBar : public QWidget {
    Q_OBJECT
public:
    explicit RibbonBar(QWidget* parent = nullptr);
    void addGroup(const QString& tabTitle, RibbonGroup* group);

private slots:
    void onTabChanged(int index);

private:
    struct TabPage {
        QWidget*     page   = nullptr;
        QHBoxLayout* layout = nullptr;
    };

    QTabBar*           m_tabBar = nullptr;
    QStackedWidget*    m_stack  = nullptr;

    QMap<QString, int> m_tabIndex;
    QMap<int, TabPage> m_pages;
};
