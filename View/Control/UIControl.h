#ifndef UICONTROL_H
#define UICONTROL_H

#include <QButtonGroup>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QRadioButton>
#include <QSlider>
#include <QWidget>

namespace s21 {
class UIControl : public QWidget {
  Q_OBJECT
 public:
  explicit UIControl(QGridLayout* layout, QWidget* parent = nullptr);
  ~UIControl() = default;
  std::vector<QLineEdit*> get_entries() { return entries; }
  std::vector<QSlider*> get_sliders() { return sliders; }
  std::vector<QLabel*> get_labels() { return labels; }

 private:
  std::vector<QSlider*> sliders;
  std::vector<QLineEdit*> entries;
  std::vector<QLabel*> labels;
  QRadioButton* parallelRadio;
  QRadioButton* perspectiveRadio;

  void setupConnection(QSlider* slider, QLineEdit* entry);

  void addEntryAndSlider(QGridLayout* layout, const QString& labelText, int row,
                         int& col, QWidget* parent);
  void addLabelNumbers(QGridLayout* layout, int col, const QString& labelText,
                       QWidget* parent);
  void addRadioButtons(QGridLayout* layout, QWidget* parent);
  void addLabelsProjection(QGridLayout* layout, QWidget* parent);
  void setupRightPanel(QGridLayout* rightLayout, QWidget* parent);

 signals:
  void entryChanged(QLineEdit* entry);
  void projectionChanged(bool id);
};

}  // namespace s21

#endif  // UICONTROL_H