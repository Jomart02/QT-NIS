#include "ShipObject.h"
#include <QFile>

ShipObject::ShipObject( QWidget* parent) : QOpenGLWidget(parent) ,
    rotationX(0.0f), rotationY(0.0f), rotationZ(0.0f),
    cameraPitch(0.0f), cameraYaw(0.0f), cameraDistance(5.0f),
    colorR(0.5f), colorG(0.5f), colorB(0.5f) 
{ 
    
    loadOBJ(":/model/boat");
    setMouseTracking(true);
    setOrientation(0, 0, 0);
    

}

void ShipObject::loadOBJ(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open" << filename;
        return;
    }
    QTextStream in(&file);
    float minX = 1e9, maxX = -1e9, minY = 1e9, maxY = -1e9, minZ = 1e9, maxZ = -1e9;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        QStringList tokens = line.split(' ', Qt::SkipEmptyParts);
        if (tokens.isEmpty()) continue;
        if (tokens[0] == "v") {
            Vertex v = {tokens[1].toFloat(), tokens[2].toFloat(), tokens[3].toFloat()};
            vertices.push_back(v);
            minX = std::min(minX, v.x); maxX = std::max(maxX, v.x);
            minY = std::min(minY, v.y); maxY = std::max(maxY, v.y);
            minZ = std::min(minZ, v.z); maxZ = std::max(maxZ, v.z);
        } else if (tokens[0] == "vn") {
            Normal n = {tokens[1].toFloat(), tokens[2].toFloat(), tokens[3].toFloat()};
            normals.push_back(n);
        } else if (tokens[0] == "f") {
            Face f;
            for (int i = 1; i < tokens.size(); ++i) {
                QStringList indices = tokens[i].split('/');
                f.vertexIndices.push_back(indices[0].toInt() - 1);
                if (indices.size() >= 3 && !indices[2].isEmpty()) {
                    f.normalIndices.push_back(indices[2].toInt() - 1);
                } else {
                    f.normalIndices.push_back(-1);
                }
            }
            faces.push_back(f);
        }
    }
    file.close();
    center = {(minX + maxX) / 2, (minY + maxY) / 2, (minZ + maxZ) / 2};
    float maxDim = std::max({maxX - minX, maxY - minY, maxZ - minZ});
    scale = maxDim != 0 ? 3.0f / maxDim : 1.0f;
}

void ShipObject::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.2f, 0.4f, 0.8f, 1.0f); // Set background to soft blue
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    float lightPos[] = {0.0f, 1.0f, 0.0f, 0.0f}; // Directional light from above
    float diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    qDebug() << "OpenGL Version:" << QString((const char*)glGetString(GL_VERSION));
}

void ShipObject::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float fov = 45.0f;
    float aspect = w / (float)(h ? h : 1);
    float zNear = 0.1f;
    float zFar = 100.0f;
    float f = 1.0f / std::tan(fov * M_PI / 360.0f);
    float matrix[] = {
        f / aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (zFar + zNear) / (zNear - zFar), -1,
        0, 0, (2 * zFar * zNear) / (zNear - zFar), 0
    };
    glLoadMatrixf(matrix);
    glMatrixMode(GL_MODELVIEW);
}

void ShipObject::paintGL() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glRotatef(cameraPitch, 1.0f, 0.0f, 0.0f);
    glRotatef(cameraYaw, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, -cameraDistance);
    glScalef(scale, scale, scale);
    glTranslatef(-center.x, -center.y, -center.z);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Correct initial model orientation
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f); // Roll (X-axis)
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f); // Pitch (Y-axis)
    glRotatef(rotationZ, 0.0f, 0.0f, 1.0f); // Yaw (Z-axis)
    glDisable(GL_LIGHTING);
    glLineWidth(4.0f);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-2.0f, 0.0f, 0.0f);
    glVertex3f(2.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -2.0f, 0.0f);
    glVertex3f(0.0f, 2.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -2.0f);
    glVertex3f(0.0f, 0.0f, 2.0f);
    glEnd();
    glEnable(GL_LIGHTING);
    glColor3f(colorR, colorG, colorB); // Use dynamic color
    for (const auto& face : faces) {
        glBegin(GL_POLYGON);
        for (size_t i = 0; i < face.vertexIndices.size(); ++i) {
            int nIdx = face.normalIndices[i];
            if (nIdx >= 0 && nIdx < static_cast<int>(normals.size())) {
                glNormal3f(normals[nIdx].x, normals[nIdx].y, normals[nIdx].z);
            }
            int vIdx = face.vertexIndices[i];
            glVertex3f(vertices[vIdx].x, vertices[vIdx].y, vertices[vIdx].z);
        }
        glEnd();
    }
}

void ShipObject::mousePressEvent(QMouseEvent *event) {
    lastMousePos = event->pos();
}

void ShipObject::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        float dx = event->x() - lastMousePos.x();
        float dy = event->y() - lastMousePos.y();

        cameraYaw += dx * 0.5f;
        cameraPitch += dy * 0.5f;

        // Clamp pitch to avoid flipping
        if (cameraPitch > 89.0f) cameraPitch = 89.0f;
        if (cameraPitch < -89.0f) cameraPitch = -89.0f;

        lastMousePos = event->pos();
        update();
    }
}

void ShipObject::wheelEvent(QWheelEvent *event) {
    cameraDistance -= event->angleDelta().y() / 120.0f * 0.5f;
    if (cameraDistance < 0.1f) cameraDistance = 0.1f;
    update();
}

void ShipObject::setRotationX(int angle) {
    rotationX = static_cast<float>(angle);
    update();
}

void ShipObject::setRotationY(int angle) {
    rotationY = static_cast<float>(angle);
    update();
}

void ShipObject::setRotationZ(int angle) {
    rotationZ = static_cast<float>(angle);
    update();
}

void ShipObject::setOrientation(int roll, int pitch, int yaw) {
    rotationX = static_cast<float>(roll);
    rotationY = static_cast<float>(pitch);
    rotationZ = static_cast<float>(yaw);
    qDebug() << "Set orientation: roll =" << roll << "pitch =" << pitch << "yaw =" << yaw;
    update();
}

void ShipObject::setColor(float r, float g, float b) {
    colorR = r;
    colorG = g;
    colorB = b;
    qDebug() << "Set color: R =" << r << "G =" << g << "B =" << b;
    update();
}