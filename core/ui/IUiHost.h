#pragma once

#include <QString>

#include "GridLayoutDef.h"

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
    // tabTitle служит одновременно отображаемым названием вкладки и ключом:
    //   - Первый вызов с данным заголовком → создаёт новую вкладку.
    //   - Повторный вызов с тем же заголовком → группа добавляется в неё.
    //
    // Это позволяет нескольким плагинам разделить одну вкладку:
    //   host->addRibbonGroup("Главная", myGroup);  // войдёт в вкладку "Главная"
    //   host->addRibbonGroup("Карта",   myGroup);  // создаст свою вкладку
    //
    // После вызова group становится дочерним виджетом ленты —
    // Qt удалит его автоматически при уничтожении окна.
    // Самостоятельно вызывать delete group не нужно.
    virtual void addRibbonGroup(const QString& tabTitle,
                                RibbonGroup*   group) = 0;

    // ── Сетка (GridWidget) ───────────────────────────────────────────────────

    // Применяет раскладку к центральной сетке GridWidget.
    // Все текущие виджеты в сетке удаляются; плагины получают сигнал removeClicked.
    virtual void applyGridLayout(const GridLayoutDef& def) = 0;

    virtual void showSettings(const QString& title, QWidget* settings) = 0;
    virtual void addStatusBarWidget(QWidget* widget, int stretch = 0) = 0;
    virtual void addMenuAction(const QString& menuTitle, QAction* action) = 0;
};
