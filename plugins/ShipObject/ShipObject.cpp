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
   // Очистка буферов цвета и глубины
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Сброс матрицы вида
    glLoadIdentity();

    // --- 1. ТРАНСФОРМАЦИЯ КАМЕРЫ ---
    // Сначала отодвигаем камеру назад
    glTranslatef(0.0f, 0.0f, -cameraDistance);
    
    // Вращаем камеру вокруг центра (орбитальное вращение)
    glRotatef(cameraPitch, 1.0f, 0.0f, 0.0f);
    glRotatef(cameraYaw, 0.0f, 1.0f, 0.0f);
    
    // Смещаем мир относительно точки обзора (панорамирование)
    // Если вы не используете правую кнопку мыши, cameraTarget будет (0,0,0)
    glTranslatef(-cameraTarget.x(), -cameraTarget.y(), -cameraTarget.z());

    // --- 2. СЕТКА ИЛИ ОСИ КООРДИНАТ (опционально) ---
    glDisable(GL_LIGHTING);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        // Ось X (Красная)
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-5.0f, 0.0f, 0.0f); glVertex3f(5.0f, 0.0f, 0.0f);
        // Ось Y (Зеленая)
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, -5.0f, 0.0f); glVertex3f(0.0f, 5.0f, 0.0f);
        // Ось Z (Синяя)
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, -5.0f); glVertex3f(0.0f, 0.0f, 5.0f);
    glEnd();
    glEnable(GL_LIGHTING);

    // --- 3. ТРАНСФОРМАЦИЯ И ОТРИСОВКА МОДЕЛИ ---
    glPushMatrix(); // Сохраняем матрицу, чтобы вращения модели не влияли на другие объекты
        
        // Приводим модель к единому масштабу и центрируем
        glScalef(scale, scale, scale);
        glTranslatef(-center.x, -center.y, -center.z);

        // Поворот самой модели (из ваших настроек/слайдеров)
        glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
        glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
        glRotatef(rotationZ, 0.0f, 0.0f, 1.0f);

        // Установка цвета материала
        glColor3f(colorR, colorG, colorB);

        // Отрисовка полигонов модели
        for (const auto& face : faces) {
            glBegin(GL_POLYGON);
            for (size_t i = 0; i < face.vertexIndices.size(); ++i) {
                // Нормаль для освещения
                int nIdx = face.normalIndices[i];
                if (nIdx >= 0 && nIdx < (int)normals.size()) {
                    glNormal3f(normals[nIdx].x, normals[nIdx].y, normals[nIdx].z);
                }
                
                // Вершина
                int vIdx = face.vertexIndices[i];
                glVertex3f(vertices[vIdx].x, vertices[vIdx].y, vertices[vIdx].z);
            }
            glEnd();
        }
    glPopMatrix(); // Возвращаем матрицу
}

void ShipObject::mousePressEvent(QMouseEvent *event) {
    lastMousePos = event->pos();
}

void ShipObject::mouseMoveEvent(QMouseEvent *event) {
   float dx = event->position().x() - lastMousePos.x();
    float dy = event->position().y() - lastMousePos.y();

    if (event->buttons() & Qt::LeftButton) {
        // Вращение (Орбита)
        cameraYaw += dx * 0.2f;
        cameraPitch += dy * 0.2f;
        
        // Ограничиваем вертикальный угол, чтобы не "перевернуться"
        cameraPitch = std::clamp(cameraPitch, -89.0f, 89.0f);

    } else if (event->buttons() & Qt::RightButton) {
        // Панорамирование (Смещение точки обзора)
        // Коэффициент 0.01f подбирается под масштаб модели
        float factor = cameraDistance * 0.001f; 
        
        // Математика для учета поворота камеры при смещении
        float radYaw = cameraYaw * M_PI / 180.0f;
        float radPitch = cameraPitch * M_PI / 180.0f;

        cameraTarget.setX(cameraTarget.x() - (dx * cos(radYaw) + dy * sin(radPitch) * sin(radYaw)) * factor);
        cameraTarget.setY(cameraTarget.y() + (dy * cos(radPitch)) * factor);
        cameraTarget.setZ(cameraTarget.z() - (dx * -sin(radYaw) + dy * sin(radPitch) * cos(radYaw)) * factor);
    }

    lastMousePos = event->pos();
    update();
}

void ShipObject::wheelEvent(QWheelEvent *event) {
   // Плавный зум, зависящий от текущей дистанции (чем ближе, тем медленнее зум)
    float delta = event->angleDelta().y() / 120.0f;
    cameraDistance -= delta * (cameraDistance * 0.1f);
    
    if (cameraDistance < 0.1f) cameraDistance = 0.1f;
    if (cameraDistance > 50.0f) cameraDistance = 50.0f;
    
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