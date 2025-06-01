// GifScreenshotDialog.cpp
#include "GifScreenshotDialog.h"
namespace s21 {

GifScreenshotDialog::GifScreenshotDialog(QWidget* parent) : ScreenshotDialog(parent) {
  setWindowTitle("Parameters GIF");
  pathEdit_->setText("View/Gif/");
}

QString GifScreenshotDialog::selectedPath() const {
    QString rawPath = pathEdit_->text();
    QFileInfo fileInfo(rawPath);

  if (fileInfo.isDir() || !fileInfo.suffix().contains("gif", Qt::CaseInsensitive)) {
    QString fileName = "gif_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".gif";
    return QDir(rawPath).filePath(fileName);
  }

  return rawPath;
}

void GifScreenshotDialog::onBrowseClicked() {
  QString fileName = "gif_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
  QString initialPath = pathEdit_->text() + fileName;

  QString filePath = QFileDialog::getSaveFileName(this,
                                                  "Выберите имя файла для сохранения GIF",
                                                  initialPath,
                                                  "GIF (*.gif)");
  if (!filePath.isEmpty()) {
    if (!filePath.endsWith(".gif", Qt::CaseInsensitive)) {
      filePath += ".gif";
    }
    pathEdit_->setText(filePath);
  }
}

}  // namespace s21
