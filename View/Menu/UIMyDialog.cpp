#include "UIMyDialog.h"

namespace s21 {
MyDialog::MyDialog(QWidget* parent, int min, int max, bool isVertixes)
    : QDialog(parent) {
  layout = new QVBoxLayout(this);

  slider = new QSlider(Qt::Horizontal, this);
  slider->setRange(min, max);
  slider->setValue(min);
  slider->setFixedWidth(100);
  layout->addWidget(slider);

  entry = new QLineEdit(this);
  entry->setPlaceholderText("1");
  entry->setFixedWidth(100);
  layout->addWidget(entry);

  buttonOk = new QPushButton("OK", this);
  layout->addWidget(buttonOk);

  layout->setAlignment(Qt::AlignHCenter);
  connect(buttonOk, &QPushButton::clicked, this, &QDialog::accept);
  connectEntrySlider(slider, entry);
  setRadioButtons(layout, isVertixes);
}

void MyDialog::setRadioButtons(QVBoxLayout* layout, bool isVertixes) {
  if (isVertixes) {
    firstRadio = new QRadioButton("&None");
    firstRadio->setChecked(true);
    secondRadio = new QRadioButton("&Squares");
    layout->addWidget(firstRadio);
    layout->addWidget(secondRadio);
    thirdRadio = new QRadioButton("&Spheres");
    layout->addWidget(thirdRadio);
  } else {
    firstRadio = new QRadioButton("&Solid");
    firstRadio->setChecked(true);
    secondRadio = new QRadioButton("&Dash");
    layout->addWidget(firstRadio);
    layout->addWidget(secondRadio);
  }
}

void MyDialog::connectEntrySlider(QSlider* slider, QLineEdit* entry) {
  QObject::connect(slider, &QSlider::valueChanged, [slider, entry, this]() {
    entry->setText(QString::number(slider->value()));
  });

  QObject::connect(entry, &QLineEdit::textChanged, [slider, entry, this]() {
    int value = entry->text().toInt();
    if (value > slider->maximum()) {
      value = slider->maximum();
      entry->setText(QString::number(value));
    } else if (value < slider->minimum()) {
      value = slider->minimum();
      entry->setText(QString::number(value));
    }
    slider->setValue(entry->text().toInt());
  });
}
}  // namespace s21