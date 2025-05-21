#ifndef SCREEN_H
#define SCREEN_H

#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QPixmap>
#include <QPushButton>
#include <QScreen>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

#include "QtGifImageLib/src/gifimage/qgifimage.h"

namespace s21 {
class GifScreenshotWidget : public QWidget {
  Q_OBJECT
friend class MainWindow;
 public:
  GifScreenshotWidget(QWidget *parent = nullptr);
  void scheduleScreenshot(const QString& baseName);

 private:
  void takeScreenshot();
  void takeIcon(const QString& baseName);

  void startRecordingGif();
  void takeGif();

 private:
  QWidget *topWidget;
  QString filePath_gif;
  QGifImage *gifImage;
  QTimer *timer;
  int count_gif_cadr;
  int limit_gif_cadr;
};
}  // namespace s21

#endif  // SCREEN_H