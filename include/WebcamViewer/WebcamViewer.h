#pragma once
#include <QWidget>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QVideoWidget>
#include <QVBoxLayout>

class WebcamViewer : public QWidget {
    Q_OBJECT

public:
    explicit WebcamViewer(QWidget *parent = nullptr);
    ~WebcamViewer();

    bool startCamera();  // Метод для запуска камеры
    void stopCamera();   // Метод для остановки камеры

private:
    QCamera *m_camera;
    QMediaCaptureSession *m_captureSession;
    QVideoWidget *m_videoWidget;
};
