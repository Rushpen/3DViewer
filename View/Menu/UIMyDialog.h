#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QLabel>

namespace s21 {
class MyDialog : public QDialog {
  Q_OBJECT
 public:
  MyDialog(QWidget *parent = nullptr, int min = 0, int max = 10,
           bool isVertixes = false);
  ~MyDialog() = default;

  QSlider *getSlider() { return slider; }
  QRadioButton *getFirstRadio() { return firstRadio; }
  QRadioButton *getSecondRadio() { return secondRadio; }
  QRadioButton *getThirdRadio() { return thirdRadio; }

 private:
  QVBoxLayout *layout;
  QSlider *slider;
  QPushButton *buttonOk;
  QLineEdit *entry;

  QRadioButton *firstRadio;
  QRadioButton *secondRadio;
  QRadioButton *thirdRadio;
  void setRadioButtons(QVBoxLayout *layout, bool isVertixes = false);
  void connectEntrySlider(QSlider *slider, QLineEdit *entry);
};
}  // namespace s21
