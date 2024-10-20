#include "Screen.h"

namespace s21 {
GifScreenshotWidget::GifScreenshotWidget(QWidget *parent) : QWidget(parent) {
  timer = new QTimer(this);
  QPushButton *button_screen = new QPushButton("Screen", this);
  connect(button_screen, &QPushButton::clicked, this,
          &GifScreenshotWidget::takeScreenshot);

  limit_gif_cadr = 50;
  QPushButton *button_gif = new QPushButton("Gif", this);
  connect(button_gif, &QPushButton::clicked, this,
          &GifScreenshotWidget::startRecordingGif);
  QVBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(button_screen);
  layout->addWidget(button_gif);
  setLayout(layout);
}

void GifScreenshotWidget::takeScreenshot() {
  QDir dir("View/Screenshots");
  if (!dir.exists()) {
    dir.mkpath(".");
  }
  QWidget *topWidget = QApplication::activeWindow();
  QPixmap screenshot = topWidget->grab();

  QString fileName =
      "screenshot_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
  QString filePath = QFileDialog::getSaveFileName(this, "Save Screenshot",
                                                  dir.path() + "/" + fileName,
                                                  "JPEG (*.jpeg);;BMP (*.bmp)");
  if (!filePath.isEmpty()) {
    if (filePath.endsWith(".bmp"))
      screenshot.save(filePath, "BMP");
    else
      screenshot.save(filePath, "JPEG");
  }
}

void GifScreenshotWidget::startRecordingGif() {
  QDir dir("View/Gif");
  if (!dir.exists()) {
    dir.mkpath(".");
  }
  topWidget = QApplication::activeWindow();
  topWidget->setFocus();
  QString fileName =
      "gif_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
  filePath_gif = QFileDialog::getSaveFileName(
      this, "Save Gif", dir.path() + "/" + fileName, "GIF (*.gif)");

  if(!filePath_gif.isEmpty()) {
    count_gif_cadr = 0;
    timer->start(100);
    gifImage = new QGifImage();
    connect(timer, &QTimer::timeout, this, &GifScreenshotWidget::takeGif);
  }
}

void GifScreenshotWidget::takeGif() {
  if (count_gif_cadr < limit_gif_cadr) {
    QPixmap screenshot = topWidget->grab();
    QImage img = screenshot.toImage();
    QImage imgSize = img.scaled(QSize(640, 480));
    gifImage->addFrame(imgSize, 100);
    count_gif_cadr++;

  } else {
    timer->stop();
    gifImage->save(filePath_gif);
    topWidget->clearFocus();
    disconnect(timer, &QTimer::timeout, this, &GifScreenshotWidget::takeGif);
    delete gifImage;
  }
}
}  // namespace s21
