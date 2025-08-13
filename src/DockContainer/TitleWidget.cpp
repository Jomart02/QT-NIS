#include "TitleWidget.h"
#include "ui_TitleWidget.h"
#include <QMainWindow>
TitleWidget::TitleWidget(QString titleName, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::TitleWidget),
    parent(parent)
{
    ui->setupUi(this);
    setTitle(titleName);
    setMouseTracking(true);
    connect(ui->close, &QPushButton::clicked, this, &TitleWidget::close);
}

void TitleWidget::setTitle(const QString& title)
{
    ui->name->setText(title);
}

void TitleWidget::showEvent(QShowEvent* event)
{
	QWidget::showEvent(event);
	emit show();
}

void TitleWidget::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton) {
        oldMousePoint = event->pos();
        parent->raise();
        emit click();
    }
}
void TitleWidget::mouseDoubleClickEvent(QMouseEvent *event){
    emit d_click();
    stateVis = !stateVis;
    QWidget::mouseDoubleClickEvent(event);
}
void TitleWidget::mouseMoveEvent(QMouseEvent* event)
{
    // QPoint delta = event->pos() - oldMousePoint;
    // parent->move(parent->pos() + delta);

    if (event->buttons() & Qt::LeftButton) {
        QPoint delta = event->pos() - oldMousePoint;
        QPoint newPos = parentWidget()->pos() + delta;

        auto mainWindow = qobject_cast<QMainWindow*>(parentWidget()->parentWidget());
        if (mainWindow) {
            QRect mainWindowRect = mainWindow->rect();
            QRect dockWidgetRect = parentWidget()->rect();

            int newX = qMax(mainWindowRect.left(), qMin(newPos.x(), mainWindowRect.right() - dockWidgetRect.width()));
            int newY = qMax(mainWindowRect.top(), qMin(newPos.y(), mainWindowRect.bottom() - dockWidgetRect.height()));

            if (parentWidget()->pos() != QPoint(newX, newY)) {
                parentWidget()->move(newX, newY);
            }
        }else{
            parent->move(parent->pos() + delta);
        }
    }

}


TitleWidget::~TitleWidget()
{

}
