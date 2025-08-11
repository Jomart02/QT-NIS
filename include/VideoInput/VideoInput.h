#pragma once

#include <QWidget>
#include <qwidget.h>
#include "ToolWidgetBase.h"



namespace Ui
{
    class VideoInput;
}


class VideoInput : public ToolWidgetBase{
    Q_OBJECT
public:
    VideoInput(QWidget *parent = nullptr);
    ~VideoInput();

private:
    Ui::VideoInput* ui;
};