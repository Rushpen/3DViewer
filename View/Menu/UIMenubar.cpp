#include "UIMenubar.h"
namespace s21 {

MenuBarWidget::MenuBarWidget(Controller *controller, QWidget *parent)
    : QWidget(parent), controller(controller) {
  setupMenu();
  setupConnections();
  loadRecentFilesFromSettings();
}

void MenuBarWidget::setupMenu() {
  menuBar = new QMenuBar(this);
  MainMenu = new QMenu("File", this);
  RecordMenu = new QMenu("Screen/Gif", this);
  PreferencesMenu = new QMenu("Edit", this);

  RecentFilesMenu = new QMenu("Recent Files", this);
  RecentFilesMenu->setStyleSheet(styleRecentFiles);
  MainMenu->addMenu(RecentFilesMenu);

  EdgesMenu = new QMenu("Edges");
  EdgesMenu->setEnabled(false);

  VertixesMenu = new QMenu("Vertixes");
  VertixesMenu->setEnabled(false);

  gifScreenshotWidget = new GifScreenshotWidget();

  openFileAction = MainMenu->addAction("Open File");
  exportFileMenu = new QMenu("Export to");
  MainMenu->addMenu(exportFileMenu);
  exportOBJAction = exportFileMenu->addAction(".obj");
  exportSTLAction = exportFileMenu->addAction(".stl");
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
      QFileDialog::getOpenFileName(this, "Open File", "", "3D Files (*.obj *.stl);;All Files (*.*)");

  if (fullFileName.isEmpty()) return;

    QString inputPath;
    if (!prepareInputFile(fullFileName, inputPath)) { return; }
    currentFilePath = inputPath;

    processModel(inputPath, fullFileName);
}

bool MenuBarWidget::prepareInputFile(const QString& originalFilePath, QString& outInputPath) {
    QFileInfo fileInfo(originalFilePath);
    QString suf = fileInfo.suffix().toLower();

    if (suf == "obj") {
        outInputPath = originalFilePath;
        return true;
    }
    else if (suf == "stl") {
        QString tempObjPath;
        if (!ModelIO::convertStlToObj(originalFilePath, tempObjPath)) {
            QMessageBox::warning(this, "Ошибка", "Не удалось конвертировать STL в OBJ");
            return false;
        }
        outInputPath = tempObjPath;
        return true;
    }
    else {
        QMessageBox::warning(this, "Ошибка", "Формат не поддерживается: " + suf);
        return false;
    }
}

void MenuBarWidget::processModel(const QString& inputPath, const QString& originalFilePath) {
    QFileInfo fileInfo(originalFilePath);

    fileName = fileInfo.fileName();
    controller->start(inputPath.toStdString());

    std::vector<S21Matrix> points = controller->get_point();
    std::set<segment> faces = controller->get_face();

    QMessageBox::information(this, "File Selected", "File: " + originalFilePath);
    updateRecentFiles(originalFilePath);

    ModelData model;
    model.name = fileInfo.completeBaseName();
    model.verticesCount = points.size();
    model.edgesCount = faces.size();

    int modelId = SettingsManager::saveOrFindModel(model);
    emit modelIdChanged(modelId);
    emit fileLoaded(points, faces, modelId);

    RecentFileInfo info;
    info.filename = originalFilePath;
    info.openedAt = QDateTime::currentDateTime();
    SettingsManager::insertRecentFile(info, model);
}

void MenuBarWidget::setupConnections() {
  connect(exitAction, &QAction::triggered, this, &QWidget::close);
  connect(openFileAction, &QAction::triggered, this,
          &MenuBarWidget::openFileMenu);
  connect(exportOBJAction, &QAction::triggered, this, &MenuBarWidget::exportModelTo);
  connect(exportSTLAction, &QAction::triggered, this, &MenuBarWidget::exportModelTo);
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

void MenuBarWidget::updateRecentFiles(const QString &file) {
  recentFiles.removeAll(file);
  recentFiles.prepend(file);
  if (recentFiles.size() > maxRecentFiles)
    recentFiles.removeLast();

  rebuildRecentMenu();
}

void MenuBarWidget::rebuildRecentMenu() {
  RecentFilesMenu->clear();
  for (const QString &file : recentFiles) {
    QFileInfo fileInfo(file);
    QString shortName = fileInfo.fileName();
    QString baseName = fileInfo.completeBaseName();

    QAction *action = new QAction(shortName, this);
    QFont font = action->font();
    action->setFont(QFont("Arial", 12));
    action->setData(file);

    QString iconPath = QCoreApplication::applicationDirPath()
                      +"/../View/Screenshots/icons/" + baseName + ".jpeg";
    action->setIcon(QIcon(iconPath));

    connect(action, &QAction::triggered, this, [this, action]() {
      QString fullPath = action->data().toString();
      controller->start(fullPath.toStdString());
      std::vector<S21Matrix> points = controller->get_point();
      std::set<segment> faces = controller->get_face();

      ModelData model;
      model.name = QFileInfo(fullPath).completeBaseName();
      model.verticesCount = points.size();
      model.edgesCount = faces.size();

      int modelId = SettingsManager::saveOrFindModel(model);
      emit modelIdChanged(modelId);
      emit fileLoaded(points, faces, modelId);
    });
    RecentFilesMenu->addAction(action);
  }
}

void MenuBarWidget::loadRecentFilesFromSettings() {
  QVector<RecentFileInfo> savedFiles = SettingsManager::loadRecentFiles();
  recentFiles.clear();
  for (const RecentFileInfo &info : savedFiles) {
    recentFiles.append(info.filename);
  }
  rebuildRecentMenu();
}

void MenuBarWidget::exportModelTo() {
  QAction* action = qobject_cast<QAction*>(sender());
  if (!action || currentFilePath.isEmpty()) return;

  QString format, filter;
  if (action == exportOBJAction) {
    format = "obj";
    filter = "OBJ Files (*.obj)";
  } else if (action == exportSTLAction) {
    format = "stl";
    filter = "STL Files (*.stl)";
  } else { return; }

  QString fileName = QFileDialog::getSaveFileName(this, "Export Model", "", filter);
  if (fileName.isEmpty()) return;

  if (!fileName.endsWith("." + format, Qt::CaseInsensitive)) {
    fileName += "." + format;
  }

  if (ModelIO::exportModel(currentFilePath, format, fileName)) {
    QMessageBox::information(this, "Export", "Successfully exported to " + fileName);
  } else {
    QMessageBox::warning(this, "Export Error", "Failed to export to " + format.toUpper());
  }
}

}  // namespace s21