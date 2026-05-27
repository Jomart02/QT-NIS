#pragma once

#include <QWidget>
#include <QString>

class QHBoxLayout;


//   ┌──────────────────────────────────┐
//   │  [widget1]  [widget2]  [widget3] │  ← content row 
//   ├──────────────────────────────────┤  ← thin separator line
//   │          Group Title             │  ← title label
//   └──────────────────────────────────┘
class RibbonGroup : public QWidget {
    Q_OBJECT
public:
    explicit RibbonGroup(const QString& title, QWidget* parent = nullptr);

    // Append a widget into the content row (left-aligned, before the stretch).
    void addWidget(QWidget* widget);

    // Append a thin vertical separator line inside the content row.
    void addSeparator();

    QString title() const { return m_title; }

private:
    QString      m_title;
    QHBoxLayout* m_contentLayout = nullptr;
};
