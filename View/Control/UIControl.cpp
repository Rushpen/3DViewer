#include "UIControl.h"
namespace s21 {
UIControl::UIControl(QGridLayout* layout, QWidget* parent) : QWidget(parent) {
  setupRightPanel(layout, parent);
}

void UIControl::setupRightPanel(QGridLayout* rightLayout, QWidget* parent) {
  int col = 0;
  addEntryAndSlider(rightLayout, "Coordinates:", 0, col, parent);
  addEntryAndSlider(rightLayout, "Rotate:", 1, col, parent);
  addEntryAndSlider(rightLayout, "Scale:", 2, col, parent);

  addLabelNumbers(rightLayout, 1, "File: ", parent);
  addLabelNumbers(rightLayout, 3, "Vertixes: ", parent);
  addLabelNumbers(rightLayout, 5, "Edges: ", parent);

  addRadioButtons(rightLayout, parent);
}

void UIControl::setupConnection(QSlider* slider, QLineEdit* entry) {
  QObject::connect(slider, &QSlider::sliderReleased, [slider, entry, this]() {
    entry->setText(QString::number(slider->value() / 10.0, 'f', 1));
    emit entryChanged(entry);
  });

  QObject::connect(entry, &QLineEdit::textChanged, [slider, entry, this]() {
    double value = entry->text().toDouble();
    if (value > slider->maximum()) {
      value = slider->maximum();
      entry->setText(QString::number(value));
    } else if (value < slider->minimum()) {
      value = slider->minimum();
      entry->setText(QString::number(value));
    }
    slider->setValue(entry->text().toDouble() * 10);
    emit entryChanged(entry);
  });
}

void UIControl::addEntryAndSlider(QGridLayout* layout, const QString& labelText,
                                  int row, int& col, QWidget* parent) {
  QLabel* label = new QLabel(labelText, parent);
  label->setFixedWidth(90);
  label->setFixedHeight(20);
  layout->addWidget(label, row, col);

  int countEntry = (row == 2) ? 1 : 3;
  for (int i = 0; i < countEntry; ++i) {
    QSlider* slider = new QSlider(Qt::Horizontal, parent);
    slider->setRange(row == 1 ? -1800 : (row == 2 ? 0 : -1000),
                     row == 1 ? 1800 : (row == 2 ? 50 : 1000));
    slider->setFixedWidth(80);
    slider->setSingleStep(row == 2 ? 1 : 10);
    layout->addWidget(slider, row, col + 1 + i * 2);

    QLineEdit* entry = new QLineEdit(parent);
    entry->setPlaceholderText("0");
    entry->setFixedWidth(50);
    layout->addWidget(entry, row, col + 2 + i * 2);

    setupConnection(slider, entry);
    slider->setEnabled(false);
    entry->setEnabled(false);
    sliders.push_back(slider);
    entries.push_back(entry);
  }
}

void UIControl::addLabelNumbers(QGridLayout* layout, int col,
                                const QString& labelText, QWidget* parent) {
  QLabel* label = new QLabel(labelText, parent);
  label->setFixedWidth(120);
  label->setFixedHeight(20);
  layout->addWidget(label, 5, col);
  labels.push_back(label);
}

void UIControl::addLabelsProjection(QGridLayout* layout, QWidget* parent) {
  QLabel* label = new QLabel("            Projection: ", parent);
  label->setFixedWidth(150);
  label->setFixedHeight(20);
  layout->addWidget(label, 2, 3);
}

void UIControl::addRadioButtons(QGridLayout* layout, QWidget* parent) {
  addLabelsProjection(layout, parent);
  parallelRadio = new QRadioButton("&Parallel");
  parallelRadio->setChecked(true);
  perspectiveRadio = new QRadioButton("&Perspective");
  layout->addWidget(parallelRadio, 2, 4);
  layout->addWidget(perspectiveRadio, 2, 5);

  connect(parallelRadio, &QRadioButton::toggled, this, [=](bool checked) {
    if (checked) {
      emit projectionChanged(true);
    }
  });

  connect(perspectiveRadio, &QRadioButton::toggled, this, [=](bool checked) {
    if (checked) {
      emit projectionChanged(false);
    }
  });
}

}  // namespace s21