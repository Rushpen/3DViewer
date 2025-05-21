#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QWidget>

#include "../3DViewer_back/mc/controller.h"
#include "Control/UIControl.h"
#include "Menu/UIMenubar.h"
#include "Scene/UIScene.h"

namespace s21 {

class MainWindow : public QWidget {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

  void updateScene(const std::vector<S21Matrix> points,
                   const std::set<segment> faces);
  void updateStates(QLineEdit* entry);
  UIScene* getSceneWidget() { return Scene; }

 private:
  UIScene* Scene;
  UIControl* RightPanel;
  MenuBarWidget* menuBarWidget;
  Controller* controller;
  QHBoxLayout* mainLayout;
  QGridLayout* rightLayout;
  std::vector<double> previousSliderValues;

  void setupUI();
  void printVertixesAndEdgesNumbers();
  void changeBackgroundColor(const QColor& color);
  void changeVertixesColor(const QColor& color);
  void changeEdgesColor(const QColor& color);
  void changeEdgesSettings(const int& width, const bool& type);
  void changeVertixesSettings(const int& size, const int& type);
  void changeSceneProjection(bool id);

  void unlockEntriesAndSliders();
  void unlockMenus();
  void unlock();
  void closeEvent(QCloseEvent* event);
};

}  // namespace s21

#endif  // MAINWINDOW_H