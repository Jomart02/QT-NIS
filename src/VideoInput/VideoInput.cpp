#include "VideoInput.h"
#include "globalVar.h"
#include "ui_VideoInput.h"

VideoInput::VideoInput(QWidget *parent) :ToolWidgetBase(WindowsDef::WindowId::VideoInput, parent), ui(new Ui::VideoInput) {
    setupUi(ui);
}
VideoInput::~VideoInput(){

}