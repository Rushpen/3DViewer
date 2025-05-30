#include <QSize>
#include <QDialog>
#include <QSpinBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFormLayout>
#include <QLineEdit>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QDateTime>
#include <QFileInfo>
#include <QCheckBox>
#include <QApplication>

namespace s21 {
class ScreenshotDialog : public QDialog {
  Q_OBJECT

 public:
  ScreenshotDialog(QWidget* parent = nullptr);
  ~ScreenshotDialog() = default;

  QSize selectedSize() const;
  virtual QString selectedPath() const;

 private slots:
  void onOkClicked();
  virtual void onBrowseClicked();
  void onFullscreenToggled(bool checked);
  bool isFullScreen();

 signals:
  void screenshotRequested(QSize size, QString filePath, bool isFullScreen);

 protected:
  QVBoxLayout *layout;
  QLineEdit* pathEdit_;
  QSpinBox* widthBox_;
  QSpinBox* heightBox_;
  QPushButton *buttonOk;
  QPushButton* browseButton_;
  QCheckBox* fullscreenCheckBox_ = nullptr;
};
}
