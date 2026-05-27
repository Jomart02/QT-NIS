#pragma once

#include <QList>
#include <QString>

// Описание одной ячейки в раскладке GridWidget.
// Поля соответствуют аргументам QGridLayout::addWidget(widget, row, col, rowSpan, colSpan).
struct GridCellDef {
    int row;
    int col;
    int rowSpan = 1;
    int colSpan = 1;
};

// Полное описание раскладки GridWidget.
//
// Раскладка задаёт сетку размером rows × cols и список ячеек.
// Ячейки могут занимать несколько строк/столбцов (rowSpan / colSpan > 1),
struct GridLayoutDef {
    QString            id;          // уникальный строковый ключ раскладки
    QString            displayName; // название, показываемое пользователю
    int                rows;        // число строк в сетке
    int                cols;        // число столбцов в сетке
    QList<GridCellDef> cells;       // список ячеек раскладки
};
