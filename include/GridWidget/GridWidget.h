#pragma once
#include <QWidget>
#include <QGridLayout>

#include <QMouseEvent>
#include <QEnterEvent>
#include <QPushButton>

class CellWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(QColor hoverColor READ hoverColor WRITE setHoverColor DESIGNABLE true)
public:
    explicit CellWidget(QWidget *parent = nullptr);
    ~CellWidget();


    QColor hoverColor() const { return m_hoverColor; }
    void setHoverColor(const QColor &color) { m_hoverColor = color; update(); }

    enum class WidgetState {
        Empty,    // Виджет пустой
        Occupied  // Виджет содержит дочерний элемент
    };
    enum class ShowState{
        Normal,
        Full
    };

    void setChildWidget(QWidget *widget);
    void clearChildWidget();
    bool isEmpty() const;
    bool isFull();
    void setShowState(ShowState state);
signals:
    void clickedAdd();
    void showModeClicked();
    void deleteClicked();
protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;


private:
    QWidget *m_childWidget = nullptr;
    QPushButton *m_showModeButton;
    QPushButton *m_deleteButton;
    bool m_isHovered = false;
    WidgetState m_state;
    QColor m_normalColor = Qt::white;
    QColor m_hoverColor = QColor(135, 206, 250, 100); // Полупрозрачный светло-синий
    ShowState m_stateView;
};

class GridWidget : public QWidget{
    Q_OBJECT
public:
    GridWidget(QWidget *parent = nullptr);
    ~GridWidget();
    void addWidgetToCell(int row, int col, QWidget *widget);
private slots:
    void addWidgetClicked();
    void removeWidgetClicked();
    void showFullClicked();
protected:  

private:
    void createGrid(int rows, int cols);

private:
    QGridLayout *m_gridLayout = nullptr;
};