#include "OpenGLWidget.h"

#include <GL/gl.h>
#include <GL/glu.h>

namespace s21 {
OpenGLWidget::OpenGLWidget(QWidget* parent) : QOpenGLWidget(parent) {
  setToDefaults();
  angleY = 0.0f;
  angleX = 0.0f;
  cameraRadius = 10.0f;
  useOrtho = true;
  orthoParams = {-cameraRadius * 1.5f, cameraRadius * 1.5f,
                 -cameraRadius * 1.5f, cameraRadius * 1.5f,
                 -cameraRadius * 1.5f, cameraRadius * 1.5f};
  perspectiveParams = {90.0, 1.0, 0.1, 200.0};
}

void OpenGLWidget::setToDefaults() {
  backgroundColor = QColor(0, 0, 0);
  vertixColor = QColor(0, 0, 255);
  edgeColor = QColor(255, 0, 255);
  pointSize = 5;
  edgeWidth = 2;
  isSolid = true;
  typePoint = 1;
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
  glGenBuffers(1, &lineVBO_);

  frameTimer_.start();
  lastTime_ = frameTimer_.elapsed();
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

  totalFrameCount_++;
  lastFrameCount_++;
  qint64 currentTime = frameTimer_.elapsed();

  if (currentTime - lastTime_ >= 1000) {
    double averageFPS = (totalFrameCount_ * 1000.0) / currentTime;
    qDebug() << "Текущий FPS:" << lastFrameCount_
             << "Средний FPS:" << averageFPS;

    lastFrameCount_ = 0;
    lastTime_ = currentTime;
  }
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

void OpenGLWidget::setFaces(const std::set<segment>& faces) {
  faces_ = faces;
  updateLineVBO();
}

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
  if (!pointVBOInitialized) return;

  if (typePoint == 1) {
    glPointSize(pointSize);
    glColor3f(vertixColor.redF(), vertixColor.greenF(), vertixColor.blueF());

    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
    glVertexPointer(3, GL_FLOAT, 0, nullptr);
    glDrawArrays(GL_POINTS, 0, points_.size());
    glDisableClientState(GL_VERTEX_ARRAY);
  }
  else if(typePoint == 2) {
    drawRoundPoint(16); 
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
    if (lineVertices_.empty()) return;

    glLineWidth(edgeWidth);
    glColor3f(edgeColor.redF(), edgeColor.greenF(), edgeColor.blueF());

    if (isSolid) {
      glEnableClientState(GL_VERTEX_ARRAY);

      glBindBuffer(GL_ARRAY_BUFFER, lineVBO_);
      glBufferData(GL_ARRAY_BUFFER, sizeof(float) * lineVertices_.size(),
                  lineVertices_.data(), GL_DYNAMIC_DRAW);
      glVertexPointer(3, GL_FLOAT, 0, 0);

      glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(lineVertices_.size() / 3));

      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glDisableClientState(GL_VERTEX_ARRAY);
    } else {
        for (size_t i = 0; i + 5 < lineVertices_.size(); i += 6) {
              drawDashedLine3D(
                  lineVertices_[i], lineVertices_[i + 1], lineVertices_[i + 2],
                  lineVertices_[i + 3], lineVertices_[i + 4], lineVertices_[i + 5],
                  edgeWidth
              );
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
  float scaleFactor = std::clamp(0.1f * cameraRadius, 0.005f, 2.0f);
  if (useOrtho) {
    float orthoSpan = orthoParams[1] - orthoParams[0];
    scaleFactor = std::clamp(0.02f * orthoSpan, 0.005f, 3.0f);
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
    cameraRadius = std::max(cameraRadius, 0.1f);
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
    typePoint, edgeColor, edgeWidth, isSolid,
    cameraRadius, angleX, angleY,
    orthoParams
  };
}

void OpenGLWidget::loadSettings() {
  OpenGLSettings settings;
  if (!SettingsManager::loadSettings(settings, modelId)) { 
    setToDefaults();
    return; 
  }

  backgroundColor = settings.backgroundColor;
  vertixColor = settings.vertexColor;
  pointSize = settings.vertexSize;
  typePoint = settings.vertexType;
  edgeColor = settings.edgeColor;
  edgeWidth = settings.edgeWidth;
  isSolid = settings.isSolid;

  cameraRadius = settings.cameraRadius;
  angleX = settings.cameraAngleX;
  angleY = settings.cameraAngleY;
  orthoParams = settings.orthoParams;

  update();
}

void OpenGLWidget::updatePoints(const std::vector<S21Matrix>& newPoints) {
  points_ = newPoints;

  std::vector<float> vertexData;
  vertexData.reserve(points_.size() * 3);

  for (const auto& point : points_) {
    vertexData.push_back(point(0, 0));
    vertexData.push_back(point(0, 1));
    vertexData.push_back(point(0, 2));
  }

  if (!pointVBOInitialized) {
    glGenBuffers(1, &pointVBO);
    pointVBOInitialized = true;
  }

  glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
  glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);
}

void OpenGLWidget::updateLineVBO() {
  lineVertices_.clear();

  for (const auto& face : faces_) {
    size_t i1 = face.get_segment_first() - 1;
    size_t i2 = face.get_segment_second() - 1;

    if (i1 < points_.size() && i2 < points_.size()) {
      auto& p1 = points_[i1];
      auto& p2 = points_[i2];

      lineVertices_.push_back(p1(0, 0));
      lineVertices_.push_back(p1(0, 1));
      lineVertices_.push_back(p1(0, 2));

      lineVertices_.push_back(p2(0, 0));
      lineVertices_.push_back(p2(0, 1));
      lineVertices_.push_back(p2(0, 2));
    }
  }
}


}  // namespace s21
