#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QPoint>
#include <QtCore>
#include <set>
#include <vector>

#include "../3DViewer_back/mc/controller.h"

namespace s21 {
class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  OpenGLWidget(QWidget* parent = nullptr);
  void setPoints(const std::vector<S21Matrix>& points);
  void setFaces(const std::set<segment>& faces);
  void setBackgroundColor(const QColor& color);

  void setVertixColor(const QColor& color);
  void setVertixSettings(const int& size, const int& type);
  void setEdgeColor(const QColor& color);
  void setEdgeSettings(const int& width, const bool& type);
  void changeProjection(bool useOrtho);
  void saveSettingsJson();

 protected:
  void initializeGL() override;
  void resizeGL();
  void resizeProjection(int direction, bool useOrtho);
  void paintGL() override;

 private:
  QColor backgroundColor;
  QColor vertixColor;
  QColor edgeColor;
  QPoint lastMousePos;
  std::vector<float> orthoParams;
  std::vector<float> perspectiveParams;
  std::vector<S21Matrix> points_;
  std::set<segment> faces_;
  float cameraRadius;
  float angleY;
  float angleX;
  int pointSize;
  int edgeWidth;
  bool isSolid;
  int typePoint;
  bool useOrtho;

  void wheelEvent(QWheelEvent* event) override;
  void drawLine();
  void drawPoint();
  void mousePressEvent(QMouseEvent* event);
  void mouseMoveEvent(QMouseEvent* event);

  void setProjection();
  void setCamera();
  void setOrthoProjection();
  void setPerspectiveProjection();

  void loadSettingsJson();
};
}  // namespace s21

#endif  // OPENGLWIDGET_H