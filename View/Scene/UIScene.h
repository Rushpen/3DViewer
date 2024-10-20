#ifndef UISCENE_H
#define UISCENE_H

#include <QHBoxLayout>
#include <QWidget>

#include "openGL/OpenGLWidget.h"

namespace s21 {

class UIScene : public QWidget {
 public:
  explicit UIScene(QHBoxLayout *mainLayout, QWidget *parent = nullptr);
  OpenGLWidget *getOpenGLWidget() { return openglWidget; }

 private:
  OpenGLWidget *openglWidget;
  void setupScene(QHBoxLayout *mainLayout);
};

}  // namespace s21

#endif  // UISCENE_H