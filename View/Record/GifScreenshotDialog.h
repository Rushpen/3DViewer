#pragma once
#include <QFileDialog>
#include <QDateTime>
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>

#include "ScreenshotDialog.h"

namespace s21 {

class GifScreenshotDialog : public ScreenshotDialog {
  Q_OBJECT
public:
  GifScreenshotDialog(QWidget* parent = nullptr);

  QString selectedPath() const override;

protected slots:
  void onBrowseClicked() override;
};

}  // namespace s21
