#include "UIScene.h"

namespace s21 {
UIScene::UIScene(QHBoxLayout* mainLayout, QWidget* parent) : QWidget(parent) {
  setupScene(mainLayout);
}

void UIScene::setupScene(QHBoxLayout* mainLayout) {
  openglWidget = new OpenGLWidget();
  openglWidget->setMinimumSize(500, 500);
  mainLayout->addWidget(openglWidget);
}
}  // namespace s21