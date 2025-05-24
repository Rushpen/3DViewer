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
  loadSettings();
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

void OpenGLWidget::drawRoundPoint(int segments) {
  const float radius = pointSize / 100.0f;

  for (const auto& point : points_) {
    float cx = point(0, 0);
    float cy = point(0, 1);
    float cz = point(0, 2);

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(cx, cy, cz); 
    for (int i = 0; i <= segments; ++i) {
      float angle = 2.0f * M_PI * i / segments;
      float x = cx + radius * cosf(angle);
      float y = cy + radius * sinf(angle);
      glVertex3f(x, y, cz);
    }
    glEnd();
  }
}

void OpenGLWidget::drawPoint() {
  if (typePoint == 0) return;

  glPointSize(pointSize);
  glColor3f(vertixColor.redF(), vertixColor.greenF(), vertixColor.blueF());
  
  if (typePoint == 1) {
    glBegin(GL_POINTS);
    for (const auto& point : points_) {
      glVertex3f(point(0, 0), point(0, 1), point(0, 2));
    }
    glEnd();
  } else {
    const int segments = 20;
    drawRoundPoint(segments);
  }
}

void OpenGLWidget::drawDashedLine3D(float x1, float y1, float z1, float x2,
                                    float y2, float z2, int edgeWidth) {
  const float kDash = 0.05f;
  const float kGap = 0.03f;

  float dashLength = edgeWidth * kDash;
  float gapLength = edgeWidth * kGap;

  float dx = x2 - x1;
  float dy = y2 - y1;
  float dz = z2 - z1;

  float lineLength = std::sqrt(dx*dx + dy*dy + dz*dz);
  float dashGapLength = dashLength + gapLength;

  if (lineLength < 1e-6f) return;

  float nx = dx / lineLength;
  float ny = dy / lineLength;
  float nz = dz / lineLength;

  float drawn = 0.0f;

  glBegin(GL_LINES);
  while (drawn < lineLength) {
    float start = drawn;
    float end = std::min(drawn + dashLength, lineLength);

    glVertex3f(x1 + nx * start, y1 + ny * start, z1 + nz * start);
    glVertex3f(x1 + nx * end, y1 + ny * end, z1 + nz * end);

    drawn += dashGapLength;
  }
  glEnd();
}


void OpenGLWidget::drawLine() {
  glLineWidth(edgeWidth);
  glColor3f(edgeColor.redF(), edgeColor.greenF(), edgeColor.blueF());

  for (const auto& face : faces_) {
    size_t vertx1 = face.get_segment_first() - 1;
    size_t vertx2 = face.get_segment_second() - 1;

    if (vertx1 < points_.size() && vertx2 < points_.size()) {
      auto& p1 = points_[vertx1];
      auto& p2 = points_[vertx2];

      if (isSolid) {
        glBegin(GL_LINES);
        glVertex3f(p1(0,0), p1(0,1), p1(0,2));
        glVertex3f(p2(0,0), p2(0,1), p2(0,2));
        glEnd();
      } else {
        drawDashedLine3D(p1(0,0), p1(0,1), p1(0,2),
                         p2(0,0), p2(0,1), p2(0,2),
                         edgeWidth);
      }
    }
  }
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

OpenGLSettings OpenGLWidget::getSettings() const {
  return {
    backgroundColor, vertixColor, pointSize,
    typePoint, edgeColor, edgeWidth, isSolid
  };
}

void OpenGLWidget::loadSettings() {
  OpenGLSettings settings;
  if (!SettingsManager::loadSettings(settings)) { return; }

  backgroundColor = settings.backgroundColor;
  vertixColor = settings.vertexColor;
  pointSize = settings.vertexSize;
  typePoint = settings.vertexType;
  edgeColor = settings.edgeColor;
  edgeWidth = settings.edgeWidth;
  isSolid = settings.isSolid;
}


}  // namespace s21
