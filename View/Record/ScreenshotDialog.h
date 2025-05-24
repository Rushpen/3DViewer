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

namespace s21 {
class ScreenshotDialog : public QDialog {
  Q_OBJECT

 public:
  ScreenshotDialog(QWidget* parent = nullptr);
  ~ScreenshotDialog() = default;

  QSize selectedSize() const;
  QString selectedPath() const;

 private slots:
  void onOkClicked();
  void onBrowseClicked();

 signals:
  void screenshotRequested(QSize size, QString filePath);

 private:
  QVBoxLayout *layout;
  QLineEdit* pathEdit_;
  QSpinBox* widthBox_;
  QSpinBox* heightBox_;
  QPushButton *buttonOk;
  QPushButton* browseButton_;
};
}
