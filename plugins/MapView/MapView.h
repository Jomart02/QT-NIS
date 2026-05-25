#pragma once
#include <QWidget>

class QQuickWidget;

class MapView : public QWidget
{
    Q_OBJECT
public:
    explicit MapView(QWidget *parent = nullptr);
    ~MapView();

    // Если нужно менять центр/зум снаружи
    void setCenter(double lat, double lon);
    void setZoomLevel(int zoom);

private:
    QQuickWidget *m_quickWidget;
};