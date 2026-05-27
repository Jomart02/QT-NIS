#pragma once

#include <QString>

class QWidget;
class QAction;
class RibbonGroup;
class ToolWidgetBase;

class IUiHost {
public:
    virtual ~IUiHost() = default;

    // ── Боковая панель (ToolBox) ─────────────────────────────────────────────

    // Регистрирует панель управления плагина в левом ToolBox.
    virtual void addToolPanel(const QString& id,
                              const QString& title,
                              ToolWidgetBase* panel) = 0;

    // ── Лента (Ribbon) ───────────────────────────────────────────────────────

    // Добавляет RibbonGroup на вкладку ленты с заголовком tabTitle.
    //
    // tabTitle служит одновременно отображаемым названием вкладки и ключом
    //   - Первый вызов с данным заголовком → создаёт новую вкладку.
    //   - Повторный вызов с тем же заголовком → группа добавляется в неё.
    //
    // Это позволяет нескольким плагинам разделить одну вкладку:
    //   host->addRibbonGroup("Главная",  myGroup);  // войдёт в вкладку "Главная"
    //   host->addRibbonGroup("Карта",    myGroup);  // создаст свою вкладку
    //
    // Владение: хост берёт ownership над group после вызова.
    virtual void addRibbonGroup(const QString& tabTitle,
                                RibbonGroup*   group) = 0;


    virtual void showSettings(const QString& title, QWidget* settings) = 0;
    virtual void addStatusBarWidget(QWidget* widget, int stretch = 0) = 0;
    virtual void addMenuAction(const QString& menuTitle, QAction* action) = 0;
};
