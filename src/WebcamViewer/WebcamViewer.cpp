#include "WebcamViewer.h"
#include <QCameraDevice>
#include <QMessageBox>
#include <QMediaDevices>

WebcamViewer::WebcamViewer(QWidget *parent) : QWidget(parent),
    m_camera(nullptr),
    m_captureSession(new QMediaCaptureSession(this)),
    m_videoWidget(new QVideoWidget(this)) {

    // Устанавливаем layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_videoWidget);
    layout->setContentsMargins(0, 0, 0, 0);

    // Настраиваем сессию захвата
    m_captureSession->setVideoOutput(m_videoWidget);
    startCamera();
}

WebcamViewer::~WebcamViewer() {
    stopCamera();
}

bool WebcamViewer::startCamera() {
    // Получаем доступные камеры
    const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
    if (cameras.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Нет доступных камер.");
        return false;
    }
    for (const QCameraDevice &camera : cameras) {
        qDebug() << "Camera:" << camera.description();
    }

    // Выбираем первую доступную камеру
    QCameraDevice defaultCamera = QMediaDevices::defaultVideoInput();
    m_camera = new QCamera(defaultCamera, this);
    m_captureSession->setCamera(m_camera);

    // Запускаем камеру
    m_camera->start();
    return true;
}

void WebcamViewer::stopCamera() {
    if (m_camera) {
        m_camera->stop();
        delete m_camera;
        m_camera = nullptr;
    }
}