#pragma once
#include <QWidget>

namespace Ui { class TopPanel; }

class TopPanel : public QWidget{
    Q_OBJECT
public:
     TopPanel(QWidget *parent);
    ~TopPanel();
signals:
     void exitTriggered();
     void changeVisRightPanel(bool vis);
private:
    Ui::TopPanel *ui;
};

