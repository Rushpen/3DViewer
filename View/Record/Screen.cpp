#include "Screen.h"

namespace s21 {
GifScreenshotWidget::GifScreenshotWidget(QWidget *parent) : QWidget(parent) {
  timer = new QTimer(this);
  QPushButton *button_screen = new QPushButton("Screen", this);
  connect(button_screen, &QPushButton::clicked, this,
          &GifScreenshotWidget::openScreenshotDialog);

  limit_gif_cadr = 50;
  QPushButton *button_gif = new QPushButton("Gif", this);
  connect(button_gif, &QPushButton::clicked, this,
          &GifScreenshotWidget::openGifScreenshotDialog);

  QVBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(button_screen);
  layout->addWidget(button_gif);
  setLayout(layout);
}

void GifScreenshotWidget::openScreenshotDialog() {
  s21::ScreenshotDialog dialog(this);
  connect(&dialog, &s21::ScreenshotDialog::screenshotRequested,
          this, &GifScreenshotWidget::takeScreenshot);
  
  if (dialog.exec() == QDialog::Accepted) {}
}

void GifScreenshotWidget::openGifScreenshotDialog() {
  s21::GifScreenshotDialog dialog(this);
  connect(&dialog, &s21::GifScreenshotDialog::screenshotRequested,
          this, &GifScreenshotWidget::startRecordingGif);
  
  if (dialog.exec() == QDialog::Accepted) {}
}

void GifScreenshotWidget::takeScreenshot(const QSize &size, const QString &filePath, const bool isFull) {
  if (filePath.isEmpty()) return;

  QTimer::singleShot(100, this, [=]() {
    QWidget *topWidget = QApplication::activeWindow();
    if (!topWidget) return;
    QPixmap screenshot;
    if (isFull) {
      screenshot = topWidget->grab();
    } else {
      screenshot = topWidget->grab(QRect(11, 31, size.width(), size.height()));
    }
    screenshot.save(filePath);
  });
}

void GifScreenshotWidget::startRecordingGif(const QSize &size, const QString &filePath, const bool isFull) {
  if (filePath.isEmpty()) return;
  size_ = size;
  filePath_gif = filePath;
  isFullScreen_ = isFull;
  QTimer::singleShot(150, this, [=]() {
    topWidget = QApplication::activeWindow();
    if (!topWidget) return;
    topWidget->setFocus();

    if(!filePath_gif.isEmpty()) {
      count_gif_cadr = 0;
      timer->start(100);
      gifImage = new QGifImage();
      connect(timer, &QTimer::timeout, this, &GifScreenshotWidget::takeGif);
    }
  });
}

void GifScreenshotWidget::takeGif() {
  if (count_gif_cadr < limit_gif_cadr) {
    topWidget->update();
    topWidget->repaint();
    QPixmap screenshot;
    if(isFullScreen_) {
       screenshot = topWidget->grab();
    } else {
      screenshot = topWidget->grab(QRect(11, 31, size_.width(), size_.height()));
    }
    
    QImage img = screenshot.toImage();
    gifImage->addFrame(img, 100);
    count_gif_cadr++;

  } else {
    timer->stop();
    gifImage->save(filePath_gif);
    topWidget->clearFocus();
    disconnect(timer, &QTimer::timeout, this, &GifScreenshotWidget::takeGif);
    delete gifImage;
    gifImage = nullptr;
  }
}

}  // namespace s21
