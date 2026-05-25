#include "MapViewPanel.h"
#include "MapView.h"

QWidget* MapViewPanel::getWidget()
{
    return new MapView();
}
