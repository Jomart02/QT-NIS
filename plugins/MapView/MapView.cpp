#include "MapView.h"
#include <QQuickWidget>
#include <QVBoxLayout>
#include <QQmlContext>
#include <QUrl>

MapView::MapView(QWidget *parent)
    : QWidget(parent)
{
    // Важно: используем именно QQuickWidget, а не QQuickView
    m_quickWidget = new QQuickWidget(this);
    m_quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    m_quickWidget->setSource(QUrl("qrc:/MapView.qml"));

//     // Если в QML будут ошибки — увидим их в консоли
//     connect(m_quickWidget, &QQuickWidget::statusChanged,
//             this, [](QQuickWidget::Status status){
//         if (status == QQuickWidget::Error) {
//             qWarning() << "QML errors:" << m_quickWidget->errors();
//         }
//     });

    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_quickWidget);
     m_quickWidget->setAttribute(Qt::WA_AcceptTouchEvents, true);
     m_quickWidget->grabGesture(Qt::PanGesture);
     m_quickWidget->grabGesture(Qt::PinchGesture);
     this->layout()->setContentsMargins(0, 0, 0, 0);
}

MapView::~MapView() = default;

void MapView::setCenter(double lat, double lon)
{
//     if (auto root = m_quickWidget->rootObject()) {
//         QMetaObject::invokeMethod(root, "setMapCenter",
//                                   Q_ARG(QVariant, lat),
//                                   Q_ARG(QVariant, lon));
//     }
}

void MapView::setZoomLevel(int zoom)
{
//     if (auto root = m_quickWidget->rootObject()) {
//         root->setProperty("zoomLevel", zoom);
//     }
}