#include "UIMenubar.h"
namespace s21 {

MenuBarWidget::MenuBarWidget(Controller *controller, QWidget *parent)
    : QWidget(parent), controller(controller) {
  setupMenu();
  setupConnections();
}

void MenuBarWidget::setupMenu() {
  menuBar = new QMenuBar(this);
  MainMenu = new QMenu("File", this);
  RecordMenu = new QMenu("Screen/Gif", this);
  PreferencesMenu = new QMenu("Edit", this);
  EdgesMenu = new QMenu("Edges");
  EdgesMenu->setEnabled(false);

  VertixesMenu = new QMenu("Vertixes");
  VertixesMenu->setEnabled(false);
  gifScreenshotWidget = new GifScreenshotWidget();

  openFileAction = MainMenu->addAction("Open File");
  exitAction = MainMenu->addAction("Close");

  colorBackgroundAction = PreferencesMenu->addAction("Background");
  PreferencesMenu->addMenu(EdgesMenu);
  PreferencesMenu->addMenu(VertixesMenu);

  EdgesColorAction = EdgesMenu->addAction("Color");
  EdgesTypeAction = EdgesMenu->addAction("Edit");

  VertixesColorAction = VertixesMenu->addAction("Color");
  VertixesTypeAction = VertixesMenu->addAction("Edit");

  gifScreenshotAction = new QWidgetAction(this);
  gifScreenshotAction->setDefaultWidget(gifScreenshotWidget);
  RecordMenu->addAction(gifScreenshotAction);

  menuBar->addMenu(MainMenu);
  menuBar->addMenu(PreferencesMenu);
  menuBar->addMenu(RecordMenu);
}

void MenuBarWidget::openFileMenu() {
  QString fullFileName =
      QFileDialog::getOpenFileName(this, "Open File", "", "All Files (*.*)");
  if (!fullFileName.isEmpty()) {
    QFileInfo fileInfo(fullFileName);
    fileName = fileInfo.fileName();
    controller->start(fullFileName.toStdString());
    std::vector<S21Matrix> points = controller->get_point();
    std::set<segment> faces = controller->get_face();
    QMessageBox::information(this, "File Selected", "File: " + fullFileName);
    emit fileLoaded(points, faces);
  }
}

void MenuBarWidget::setupConnections() {
  connect(exitAction, &QAction::triggered, this, &QWidget::close);
  connect(openFileAction, &QAction::triggered, this,
          &MenuBarWidget::openFileMenu);
  setupSettingsConnections();
  setupBackgroundConnection();
}

void MenuBarWidget::setupBackgroundConnection() {
  connect(colorBackgroundAction, &QAction::triggered, this, [this]() {
    QColor selectedColor =
        QColorDialog::getColor(Qt::white, this, "Choose background color");
    if (selectedColor.isValid()) {
      emit backgroundColorChanged(selectedColor);
    }
  });
}

void MenuBarWidget::setupSettingsConnections() {
  setupVertixesConnections();
  setupEdgesConnections();
}

void MenuBarWidget::setupVertixesConnections() {
  connect(VertixesColorAction, &QAction::triggered, this, [this]() {
    QColor selectedColor =
        QColorDialog::getColor(Qt::white, this, "Choose vertixe's color");
    if (selectedColor.isValid()) {
      emit vertixesColorChanged(selectedColor);
    }
  });

  connect(VertixesTypeAction, &QAction::triggered, this, [this]() {
    dialog = new MyDialog(this, 1, 20, true);
    dialog->setWindowTitle("Edit vertixes");
    dialog->setStyleSheet("background-color: lightblue;");
    if (dialog->exec() == QDialog::Accepted) {
      bool firstR = dialog->getFirstRadio()->isChecked();
      bool secondR = dialog->getSecondRadio()->isChecked();
      int size = dialog->getSlider()->value();
      int typePoint = (firstR ? 0 : (secondR ? 1 : 2));
      emit vertixesSettingsChanged(size, typePoint);
    }
    delete dialog;
  });
}

void MenuBarWidget::setupEdgesConnections() {
  connect(EdgesColorAction, &QAction::triggered, this, [this]() {
    QColor selectedColor =
        QColorDialog::getColor(Qt::white, this, "Choose edge's color");
    if (selectedColor.isValid()) {
      emit edgesColorChanged(selectedColor);
    }
  });

  connect(EdgesTypeAction, &QAction::triggered, this, [this]() {
    dialog = new MyDialog(this, 1, 10, false);
    dialog->setWindowTitle("Edit edges");
    dialog->setStyleSheet("background-color: lightblue;");
    if (dialog->exec() == QDialog::Accepted) {
      int width = dialog->getSlider()->value();
      bool typeLine = dialog->getFirstRadio()->isChecked();
      emit edgesSettingsChanged(width, typeLine);
    }
    delete dialog;
  });
}

}  // namespace s21