#include "ScreenshotDialog.h"

namespace s21 {
ScreenshotDialog::ScreenshotDialog(QWidget* parent)
    : QDialog(parent) {
  setWindowTitle("Настройки скриншота");
  layout = new QVBoxLayout(this);
  widthBox_ = new QSpinBox(this);
  heightBox_ = new QSpinBox(this);

  widthBox_->setRange(100, 10000);
  heightBox_->setRange(100, 10000);
  widthBox_->setValue(400);
  heightBox_->setValue(400);

  QLabel* widthLabel = new QLabel("Ширина (px): ", this);
  widthLabel->setAlignment(Qt::AlignHCenter);
  layout->addWidget(widthLabel);
  layout->addWidget(widthBox_);

  QLabel* heightLabel = new QLabel("Высота (px): ", this);
  heightLabel->setAlignment(Qt::AlignHCenter);
  layout->addWidget(heightLabel);
  layout->addWidget(heightBox_);

  fullscreenCheckBox_ = new QCheckBox("Всё окно", this);
  layout->addWidget(fullscreenCheckBox_, 0, Qt::AlignHCenter);

  pathEdit_ = new QLineEdit(this);
  pathEdit_->setText("View/Screenshots/");
  browseButton_ = new QPushButton("Обзор...", this);

  QHBoxLayout* pathLayout = new QHBoxLayout;
  pathLayout->addWidget(pathEdit_);
  pathLayout->addWidget(browseButton_);
  layout->addLayout(pathLayout);

  buttonOk = new QPushButton("OK", this);
  layout->addWidget(buttonOk);
  layout->setAlignment(Qt::AlignHCenter);

  connect(fullscreenCheckBox_, &QCheckBox::toggled, this, &ScreenshotDialog::onFullscreenToggled);
  connect(buttonOk, &QPushButton::clicked, this, &ScreenshotDialog::onOkClicked);
  connect(browseButton_, &QPushButton::clicked, this, &ScreenshotDialog::onBrowseClicked);
}

void ScreenshotDialog::onFullscreenToggled(bool checked) {
  widthBox_->setDisabled(checked);
  heightBox_->setDisabled(checked);
}

bool ScreenshotDialog::isFullScreen() {
  return fullscreenCheckBox_->isChecked();
}

QSize ScreenshotDialog::selectedSize() const {
  return QSize(widthBox_->value(), heightBox_->value());
}

QString ScreenshotDialog::selectedPath() const {
  QString rawPath = pathEdit_->text();
  QFileInfo fileInfo(rawPath);

  if (fileInfo.isDir() || !fileInfo.suffix().contains(QRegularExpression("jpeg|bmp", QRegularExpression::CaseInsensitiveOption))) {
    QString fileName = "screenshot_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".jpeg";
    return QDir(rawPath).filePath(fileName);
  }

  return rawPath;
}

void ScreenshotDialog::onOkClicked() {
  QSize size = selectedSize();
  QString path = selectedPath();

  QFileInfo fileInfo(path);
  QDir dir = fileInfo.absoluteDir();

  if (!dir.exists()) {
    if (!dir.mkpath(".")) {
      QMessageBox::warning(this, "Ошибка", "Не удалось создать директорию");
      return;
    }
  }

  emit screenshotRequested(size, path, isFullScreen());
  accept();
}

void ScreenshotDialog::onBrowseClicked() {
  QString fileName = "screenshot_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
  QString initialPath = pathEdit_->text() + fileName;
  
  QString selectedFilter;
  QString filePath = QFileDialog::getSaveFileName(this,
                                                  "Выберите имя файла для сохранения",
                                                  initialPath,
                                                  "JPEG (*.jpeg);;BMP (*.bmp)",
                                                  &selectedFilter);
  if (!filePath.isEmpty()) {
    if (selectedFilter.contains("BMP") && !filePath.endsWith(".bmp", Qt::CaseInsensitive)) {
      filePath += ".bmp";
    } else if (selectedFilter.contains("JPEG") && !filePath.endsWith(".jpeg", Qt::CaseInsensitive)) {
      filePath += ".jpeg";
    }
    pathEdit_->setText(filePath);
  }
}

}