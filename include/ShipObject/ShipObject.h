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
    void setOrientation(int roll, int pitch, int yaw);
    void setColor(float r, float g, float b);
private:
     void loadOBJ(const QString& filename);
private:
    struct Vertex { float x, y, z; };
    struct Normal { float x, y, z; };
    struct Face { std::vector<int> vertexIndices; std::vector<int> normalIndices; };

    QVector<Vertex> vertices;
    QVector<Normal> normals;
    QVector<Face> faces;
    Vertex center;
    float scale;
    float rotationX, rotationY, rotationZ;
    float cameraPitch, cameraYaw, cameraDistance;
    QPoint lastMousePos;
    float colorR, colorG, colorB; // Store ship color

   
};