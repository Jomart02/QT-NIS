#pragma once

#include <QWidget>
#include <QMouseEvent>

namespace Ui
{
    class TitleWidget;
}

class TitleWidget : public QWidget
{
        Q_OBJECT

    public:
        TitleWidget(QString titleName, QWidget* parent = nullptr);
        ~TitleWidget();
        void setTitle(const QString& title);
    private:
		void showEvent(QShowEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;
        void mouseDoubleClickEvent(QMouseEvent *event) override;
	signals:
		void show();
        void close();
        void click();
        void d_click();
    private:
        Ui::TitleWidget* ui;
        QWidget* parent;
        bool stateVis = true;
        QPoint oldMousePoint;
        QColor baseColor;
};