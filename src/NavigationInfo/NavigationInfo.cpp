#include "NavigationInfo.h"
#include "ui_NavigationInfo.h"

NavigationInfo::NavigationInfo(QWidget* parent): QWidget( parent), ui(new Ui::NavigationInfo){
    ui->setupUi(this);
}


NavigationInfo::~NavigationInfo(){

}
