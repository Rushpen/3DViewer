#include "MainWindow.h"
namespace s21 {

MainWindow::MainWindow(QWidget* parent) : QWidget(parent) {
  controller = new Controller();
  setWindowTitle("CPP4_3DViewer");
  setupUI();

  connect(menuBarWidget, &MenuBarWidget::fileLoaded, this,
          &MainWindow::updateScene);
  connect(RightPanel, &UIControl::entryChanged, this,
          &MainWindow::updateStates);
  connect(menuBarWidget, &MenuBarWidget::edgesColorChanged, this,
          &MainWindow::changeEdgesColor);
  connect(menuBarWidget, &MenuBarWidget::vertixesColorChanged, this,
          &MainWindow::changeVertixesColor);
  connect(menuBarWidget, &MenuBarWidget::backgroundColorChanged, this,
          &MainWindow::changeBackgroundColor);
  connect(menuBarWidget, &MenuBarWidget::edgesSettingsChanged, this,
          &MainWindow::changeEdgesSettings);
  connect(menuBarWidget, &MenuBarWidget::vertixesSettingsChanged, this,
          &MainWindow::changeVertixesSettings);
  connect(RightPanel, &UIControl::projectionChanged, this,
          &MainWindow::changeSceneProjection);
}

MainWindow::~MainWindow() {
  delete controller;
}

void MainWindow::updateScene(const std::vector<S21Matrix> points,
                             const std::set<segment> faces) {
  Scene->getOpenGLWidget()->setPoints(points);
  Scene->getOpenGLWidget()->setFaces(faces);
  Scene->getOpenGLWidget()->update();
  unlock();
  printVertixesAndEdgesNumbers();
  menuBarWidget->getGifScreenshotWidget()->scheduleScreenshot(menuBarWidget->getFilename());
}

void MainWindow::setupUI() {
  mainLayout = new QHBoxLayout(this);
  rightLayout = new QGridLayout(this);
  menuBarWidget = new MenuBarWidget(controller, this);
  Scene = new UIScene(mainLayout, this);
  RightPanel = new UIControl(rightLayout, this);

  mainLayout->setMenuBar(menuBarWidget->getMenuBar());
  mainLayout->addLayout(rightLayout);
  setLayout(mainLayout);
}

void MainWindow::updateStates(QLineEdit* entry) {
  auto entries = RightPanel->get_entries();
  State Statuses[] = {mv_X, mv_Y, mv_Z, rt_X, rt_Y, rt_Z, scal};

  for (size_t i = 0; i < entries.size(); ++i) {
    if (entries[i] == entry) {
      double newValue = entries[i]->text().toDouble();
      double delta = newValue - previousSliderValues[i];

      controller->set_status(Statuses[i]);
      if (i < 3) {
        controller->set_meaning(delta / 20);
      } else if (i >= 3 && i < 6) {
        controller->set_meaning(delta);
      } else {
        controller->set_meaning(newValue);
      }
      controller->transform();

      previousSliderValues[i] = newValue;
      break;
    }
  }
  Scene->getOpenGLWidget()->setPoints(controller->get_point());
  Scene->getOpenGLWidget()->setFaces(controller->get_face());
  Scene->getOpenGLWidget()->update();
}

void MainWindow::printVertixesAndEdgesNumbers() {
  std::vector<QLabel*> labels = RightPanel->get_labels();
  labels[0]->setText("File: " + menuBarWidget->getFilename());
  labels[1]->setText("Vertixes: " +
                     QString::number(controller->get_number_vertices()));
  labels[2]->setText("Edges: " +
                     QString::number(controller->get_number_edges()));
}

void MainWindow::changeBackgroundColor(const QColor& color) {
  getSceneWidget()->getOpenGLWidget()->setBackgroundColor(color);
}

void MainWindow::changeVertixesColor(const QColor& color) {
  getSceneWidget()->getOpenGLWidget()->setVertixColor(color);
}

void MainWindow::changeEdgesColor(const QColor& color) {
  getSceneWidget()->getOpenGLWidget()->setEdgeColor(color);
}

void MainWindow::changeEdgesSettings(const int& width, const bool& type) {
  getSceneWidget()->getOpenGLWidget()->setEdgeSettings(width, type);
}

void MainWindow::changeVertixesSettings(const int& size, const int& type) {
  getSceneWidget()->getOpenGLWidget()->setVertixSettings(size, type);
}

void MainWindow::changeSceneProjection(bool id) {
  getSceneWidget()->getOpenGLWidget()->changeProjection(id);
}

void MainWindow::unlock() {
  unlockEntriesAndSliders();
  unlockMenus();
  previousSliderValues.assign(RightPanel->get_entries().size(), 0.0);
}

void MainWindow::unlockEntriesAndSliders() {
  auto entries = RightPanel->get_entries();
  auto sliders = RightPanel->get_sliders();
  for (size_t i = 0; i < entries.size(); ++i) {
    entries[i]->setEnabled(true);
    sliders[i]->setEnabled(true);
  }
}

void MainWindow::unlockMenus() {
  auto edgesMenu = menuBarWidget->getEdgesMenu();
  auto vertixesMenu = menuBarWidget->getVertixesMenu();
  if (edgesMenu) {
    edgesMenu->setEnabled(true);
  }
  if (vertixesMenu) {
    vertixesMenu->setEnabled(true);
  }
}

void MainWindow::closeEvent(QCloseEvent* event) {
  SettingsManager::saveSettings("build/settings.json", getSceneWidget()->getOpenGLWidget()->getSettings());
  event->accept();
}
}  // namespace s21
