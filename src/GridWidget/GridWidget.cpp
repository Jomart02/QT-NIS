#include "GridWidget.h"
#include <QLayoutItem>
#include <QMouseEvent>
#include <QPainter>
#include "WebcamViewer.h"
CellWidget::CellWidget(QWidget *parent) : QWidget(parent) {
    setMouseTracking(true);
    setLayout(new QGridLayout(this));
    m_state = WidgetState::Empty;
    m_stateView = ShowState::Normal;

    m_showModeButton = new QPushButton("✎", this); 
    m_deleteButton = new QPushButton("✖", this); 
    m_showModeButton->setFixedSize(48, 48);
    m_deleteButton->setFixedSize(48, 48);

    m_showModeButton->hide();
    m_deleteButton->hide();

    
    connect(m_showModeButton, &QPushButton::clicked, this, &CellWidget::showModeClicked);
    connect(m_deleteButton, &QPushButton::clicked, this, &CellWidget::deleteClicked);
}

CellWidget::~CellWidget() {
    delete m_childWidget;
}

void CellWidget::setChildWidget(QWidget *widget) {

    if (m_childWidget) {
        delete m_childWidget;
    }
    m_childWidget = widget;
    if (m_childWidget) {
        m_childWidget->setParent(this);
        layout()->addWidget(m_childWidget);
        m_state = WidgetState::Occupied;
        m_isHovered = false;

        m_showModeButton->show();
        m_deleteButton->show();
        m_showModeButton->raise();
        m_deleteButton->raise();
        update();
    }
}

void CellWidget::clearChildWidget() {
    delete m_childWidget;
    m_childWidget = nullptr;
    m_state = WidgetState::Empty;
    m_isHovered = false;

    m_showModeButton->hide();
    m_deleteButton->hide();
    update();
}

bool CellWidget::isEmpty() const {
    return m_state == WidgetState::Empty;
}
bool CellWidget::isFull(){
    return m_stateView == ShowState::Full;
}
void CellWidget::setShowState(ShowState state){
    m_stateView = state;
}

void CellWidget::enterEvent(QEnterEvent *event) {
    if (isEmpty()) {
        m_isHovered = true;
        update();
    }
    QWidget::enterEvent(event);
}

void CellWidget::leaveEvent(QEvent *event) {
    if (isEmpty()) {
        m_isHovered = false;
        update();
    }
    QWidget::leaveEvent(event);
}

void CellWidget::mousePressEvent(QMouseEvent *event) {
    if (isEmpty()) {
        qDebug() << "Cell clicked for insertion";
        emit clickedAdd();
    }
    QWidget::mousePressEvent(event);
}

void CellWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setBrush(m_normalColor);
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect());

    if (m_isHovered && isEmpty()) {
        painter.setBrush(QBrush(m_hoverColor));

        QPen pen(QColor(30, 144, 255)); 
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        painter.setPen(pen);
        painter.drawRect(rect().adjusted(2, 2, -2, -2));

        QPolygon triangle;
        triangle << QPoint(10, 10) << QPoint(20, 10) << QPoint(15, 15);
        painter.setBrush(QColor(70, 130, 180)); 
        painter.setPen(Qt::NoPen);
        painter.drawPolygon(triangle);
    }
    
}

void CellWidget::resizeEvent(QResizeEvent *event) {

    const int margin = 5;
    const int spacing = 5;
    m_showModeButton->move(width() - m_showModeButton->width() - margin - m_deleteButton->width() - spacing, margin);
    m_deleteButton->move(width() - m_deleteButton->width() - margin, margin);
    QWidget::resizeEvent(event);
}

GridWidget::GridWidget(QWidget *parent): QWidget(parent){
    createGrid(2,2);
}
GridWidget::~GridWidget(){

}

void GridWidget::createGrid(int rows, int cols){
    if (m_gridLayout) {
        delete m_gridLayout;
        m_gridLayout = nullptr;
    }
    m_gridLayout = new QGridLayout(this);
    m_gridLayout->setSpacing(0);
    m_gridLayout->setContentsMargins(0, 0, 0, 0);

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            CellWidget *cell = new CellWidget(this);
            cell->setProperty("row", row);
            cell->setProperty("col", col);
            m_gridLayout->addWidget(cell, row, col);
            connect(cell, &CellWidget::clickedAdd, this, &GridWidget::addWidgetClicked);
            connect(cell, &CellWidget::deleteClicked, this, &GridWidget::removeWidgetClicked);
            connect(cell, &CellWidget::showModeClicked, this, &GridWidget::showFullClicked);
        }
    }

    setLayout(m_gridLayout);
}


void GridWidget::addWidgetClicked(){
    CellWidget *cell = qobject_cast<CellWidget*>(sender());
    if(cell){
        WebcamViewer *test = new WebcamViewer();
    
        cell->setChildWidget(test);
    }
}

void GridWidget::removeWidgetClicked(){
    CellWidget *cellRemove = qobject_cast<CellWidget*>(sender());
    if(cellRemove){
        cellRemove->clearChildWidget();
        if(cellRemove->isFull()){
            for(auto cell : findChildren<CellWidget*>()){
                if(cell!= cellRemove) cell->setVisible(true);
            }
            cellRemove->setShowState( CellWidget::ShowState::Normal);
        }
    }
}   

void GridWidget::showFullClicked(){
    CellWidget *cellFull = qobject_cast<CellWidget*>(sender());   
    if(cellFull){
        bool needfull = !cellFull->isFull();
        
        for(auto cell : findChildren<CellWidget*>()){
            if(cell!= cellFull) cell->setVisible(!needfull);
        }
        cellFull->setShowState(needfull ? CellWidget::ShowState::Full : CellWidget::ShowState::Normal);
    }
}