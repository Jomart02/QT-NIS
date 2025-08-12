#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <vector>
#include <QMouseEvent>
#include <QWheelEvent>

class ShipObject : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
public:
    explicit ShipObject( QWidget* parent = nullptr);
    ~ShipObject() = default;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

public slots:
    void setRotationX(int angle);
    void setRotationY(int angle);
    void setRotationZ(int angle);

private:
    struct Vertex { float x, y, z; };
    struct Normal { float x, y, z; };
    struct Face { std::vector<int> vertexIndices; std::vector<int> normalIndices; };

    std::vector<Vertex> vertices;
    std::vector<Normal> normals;
    std::vector<Face> faces;
    float rotationX = 90.0f; // Initial rotation by 90 degrees on X-axis
    float rotationY = 0.0f;
    float rotationZ = 0.0f;
    float scale = 1.0f;
    Vertex center = {0.0f, 0.0f, 0.0f};

    // Camera controls
    float cameraDistance = 5.0f;
    float cameraYaw = 0.0f;
    float cameraPitch = 0.0f;
    QPoint lastMousePos;

    void loadOBJ(const QString& filename);
};