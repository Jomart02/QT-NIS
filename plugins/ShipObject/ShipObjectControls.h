#pragma once

#include <QWidget>

namespace Ui { class ShipObjectTool; }

class ShipObjectControls : public QWidget
{
    Q_OBJECT
public:
    explicit ShipObjectControls(QWidget* parent = nullptr);
    ~ShipObjectControls() override;

signals:
    void rotationChanged(int roll, int pitch, int yaw);

private slots:
    void onSliderChanged();

private:
    Ui::ShipObjectTool* ui;
};
