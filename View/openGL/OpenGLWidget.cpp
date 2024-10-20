#include "OpenGLWidget.h"

#include <GL/gl.h>
#include <GL/glu.h>

namespace s21 {
OpenGLWidget::OpenGLWidget(QWidget* parent) : QOpenGLWidget(parent) {
  backgroundColor = QColor(0, 0, 0);
  vertixColor = QColor(0, 0, 255);
  edgeColor = QColor(255, 0, 255);
  pointSize = 5;
  edgeWidth = 2;
  isSolid = true;
  typePoint = 1;
  angleY = 0.0f;
  angleX = 0.0f;
  cameraRadius = 10.0f;
  useOrtho = true;
  orthoParams = {-cameraRadius * 1.5f, cameraRadius * 1.5f,
                 -cameraRadius * 1.5f, cameraRadius * 1.5f,
                 -cameraRadius * 1.5f, cameraRadius * 1.5f};
  perspectiveParams = {90.0, 1.0, 0.1, 200.0};
}

void OpenGLWidget::setBackgroundColor(const QColor& color) {
  backgroundColor = color;
  update();
}

void OpenGLWidget::setVertixColor(const QColor& color) {
  vertixColor = color;
  update();
}

void OpenGLWidget::setVertixSettings(const int& size, const int& type) {
  pointSize = size;
  typePoint = type;
  update();
}

void OpenGLWidget::setEdgeColor(const QColor& color) {
  edgeColor = color;
  update();
}

void OpenGLWidget::setEdgeSettings(const int& width, const bool& type) {
  edgeWidth = width;
  isSolid = type;
  update();
}

void OpenGLWidget::initializeGL() {
  initializeOpenGLFunctions();
  glClearColor(backgroundColor.redF(), backgroundColor.greenF(),
               backgroundColor.blueF(), 1.0f);
  loadSettingsJson();
}

void OpenGLWidget::resizeGL() { glViewport(0, 0, 400, 200); }

void OpenGLWidget::paintGL() {
  glClearColor(backgroundColor.redF(), backgroundColor.greenF(),
               backgroundColor.blueF(), 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  setProjection();
  setCamera();

  if (!(points_.empty())) {
    drawPoint();
    drawLine();
  };
}

void OpenGLWidget::setProjection() {
  if (useOrtho) {
    ;
    setOrthoProjection();
  } else {
    setPerspectiveProjection();
  }
}

void OpenGLWidget::setCamera() {
  float cameraX = cameraRadius * sin(qDegreesToRadians(angleY)) *
                  cos(qDegreesToRadians(angleX));
  float cameraY = cameraRadius * sin(qDegreesToRadians(angleX));
  float cameraZ = cameraRadius * cos(qDegreesToRadians(angleY)) *
                  cos(qDegreesToRadians(angleX));

  gluLookAt(cameraX, cameraY, cameraZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
}

void OpenGLWidget::changeProjection(bool useOrtho) {
  this->useOrtho = useOrtho;
  update();
}

void OpenGLWidget::setPoints(const std::vector<S21Matrix>& points) {
  points_ = points;
}

void OpenGLWidget::setFaces(const std::set<segment>& faces) { faces_ = faces; }

void OpenGLWidget::drawPoint() {
  if (typePoint == 0) return;
  glPointSize(pointSize);
  glColor3f(vertixColor.redF(), vertixColor.greenF(), vertixColor.blueF());
  if (typePoint == 2) {
    glEnable(GL_POINT_SMOOTH);
  } else {
    glDisable(GL_POINT_SMOOTH);
  }
  glBegin(GL_POINTS);
  for (const auto& point : points_) {
    glVertex3f(point(0, 0), point(0, 1), point(0, 2));
  }
  glEnd();
}

void OpenGLWidget::drawLine() {
  glLineWidth(edgeWidth);
  glColor3f(edgeColor.redF(), edgeColor.greenF(), edgeColor.blueF());
  if (!isSolid) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x00FF);
  } else {
    glDisable(GL_LINE_STIPPLE);
  }
  glBegin(GL_LINES);
  for (const auto& face : faces_) {
    const auto& vertx1 = face.get_segment_first() - 1;
    const auto& vertx2 = face.get_segment_second() - 1;
    if (vertx1 < points_.size() && vertx2 < points_.size()) {
      glVertex3f(points_[vertx1](0, 0), points_[vertx1](0, 1),
                 points_[vertx1](0, 2));
      glVertex3f(points_[vertx2](0, 0), points_[vertx2](0, 1),
                 points_[vertx2](0, 2));
    }
  }

  glEnd();
}

void OpenGLWidget::setOrthoProjection() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(orthoParams[0], orthoParams[1], orthoParams[2], orthoParams[3],
          orthoParams[4], orthoParams[5]);
}

void OpenGLWidget::setPerspectiveProjection() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(perspectiveParams[0], perspectiveParams[1],
                 perspectiveParams[2], perspectiveParams[3]);
}

void OpenGLWidget::mousePressEvent(QMouseEvent* event) {
  if (event->button() == Qt::RightButton) {
    lastMousePos = event->pos();
  }
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event) {
  if (event->buttons() & Qt::RightButton) {
    float dx = event->x() - lastMousePos.x();
    float dy = event->y() - lastMousePos.y();

    angleY += dx * 0.1f;
    angleX -= dy * 0.1f;

    lastMousePos = event->pos();
    update();
  }
}

void OpenGLWidget::resizeProjection(int direction, bool useOrtho) {
  float scaleFactor = 0.1f;
  if (useOrtho) {
    if (direction > 0) {
      orthoParams[0] += scaleFactor;
      orthoParams[1] -= scaleFactor;
      orthoParams[2] += scaleFactor;
      orthoParams[3] -= scaleFactor;
    } else {
      orthoParams[0] -= scaleFactor;
      orthoParams[1] += scaleFactor;
      orthoParams[2] -= scaleFactor;
      orthoParams[3] += scaleFactor;
    }
    setOrthoProjection();
  } else {
    if (direction > 0) {
      cameraRadius -= scaleFactor;
    } else if (direction < 0) {
      cameraRadius += scaleFactor;
    }
    setPerspectiveProjection();
  }
}

void OpenGLWidget::wheelEvent(QWheelEvent* event) {
  int delta = event->angleDelta().y();
  resizeProjection(delta > 0 ? 1 : -1, useOrtho);
  update();
}

void OpenGLWidget::saveSettingsJson() {
  QJsonObject settings;
  settings["background_color"] = backgroundColor.name();
  settings["vertix_Color"] = vertixColor.name();
  settings["vertix_size"] = pointSize;
  settings["vertix_type"] = typePoint;
  settings["edge_Color"] = edgeColor.name();
  settings["edge_width"] = edgeWidth;
  settings["edge_is_solid"] = isSolid;

  QFile file("build/settings.json");
  if (file.open(QIODevice::WriteOnly)) {
    file.write(QJsonDocument(settings).toJson());
    file.close();
  }
}

void OpenGLWidget::loadSettingsJson() {
  QFile file("build/settings.json");
  if (file.open(QIODevice::ReadOnly)) {
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject settings = doc.object();

    backgroundColor = settings.value("background_color").toString();
    vertixColor = settings.value("vertix_Color").toString();
    pointSize = settings.value("vertix_size").toInt();
    typePoint = settings.value("vertix_type").toInt();
    edgeColor = settings.value("edge_Color").toString();
    edgeWidth = settings.value("edge_width").toInt();
    isSolid = settings.value("edge_is_solid").toBool();
    file.close();
  }
}

}  // namespace s21
