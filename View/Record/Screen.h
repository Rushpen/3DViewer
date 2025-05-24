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
#include <QSize>

#include "QtGifImageLib/src/gifimage/qgifimage.h"
#include "GifScreenshotDialog.h"

namespace s21 {
class GifScreenshotWidget : public QWidget {
  Q_OBJECT
friend class MainWindow;
 public:
  GifScreenshotWidget(QWidget *parent = nullptr);
  void scheduleScreenshot(const QString& baseName);

 private:
  void openScreenshotDialog();
  void openGifScreenshotDialog();

  void takeScreenshot(const QSize &size, const QString &filePath, const bool isFull);
  void takeIcon(const QString& baseName);

  void startRecordingGif(const QSize &size, const QString &filePath, const bool isFull);
  void takeGif();

 private:
  QSize size_;
  bool isFullScreen_;
  QString filePath_gif;
  QWidget *topWidget;
  QGifImage *gifImage;
  QTimer *timer;
  int count_gif_cadr;
  int limit_gif_cadr;
};
}  // namespace s21

#endif  // SCREEN_H