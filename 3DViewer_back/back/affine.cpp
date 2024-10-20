#include "affine.h"

namespace s21 {

void move_X::tf_model(std::vector<S21Matrix> &point, const double meaning) {
  for (unsigned i = 0; i < point.size(); i++) {
    point[i](0, 0) += meaning;
  }
}

void move_Y::tf_model(std::vector<S21Matrix> &point, const double meaning) {
  for (unsigned i = 0; i < point.size(); i++) {
    point[i](0, 1) += meaning;
  }
}

void move_Z::tf_model(std::vector<S21Matrix> &point, const double meaning) {
  for (unsigned i = 0; i < point.size(); i++) {
    point[i](0, 2) += meaning;
  }
}

void rotate_X::tf_model(std::vector<S21Matrix> &point, const double meaning) {
  double angle = meaning * M_PI / 180;
  S21Matrix uniform(3, 3);

  uniform(0, 0) = 1;
  uniform(1, 1) = cos(angle);
  uniform(1, 2) = sin(angle);
  uniform(2, 1) = -sin(angle);
  uniform(2, 2) = cos(angle);

  for (unsigned i = 0; i < point.size(); i++) {
    point[i].MulMatrix(uniform);
  }
}

void rotate_Y::tf_model(std::vector<S21Matrix> &point, const double meaning) {
  double angle = meaning * M_PI / 180;

  for (unsigned i = 0; i < point.size(); i++) {
    double x = point[i](0, 0);
    double z = point[i](0, 2);
    point[i](0, 0) = x * cos(angle) + z * sin(angle);
    point[i](0, 2) = z * cos(angle) - x * sin(angle);
  }
}

void rotate_Z::tf_model(std::vector<S21Matrix> &point, const double meaning) {
  double angle = meaning * M_PI / 180;
  S21Matrix uniform(3, 3);

  uniform(0, 0) = cos(angle);
  uniform(0, 1) = sin(angle);
  uniform(1, 0) = -sin(angle);
  uniform(1, 1) = cos(angle);
  uniform(2, 2) = 1;

  for (unsigned i = 0; i < point.size(); i++) {
    point[i].MulMatrix(uniform);
  }
}

void scale::tf_model(std::vector<S21Matrix> &point, const double meaning) {
  if (meaning > 0) {
    S21Matrix uniform(3, 3);

    uniform(0, 0) = meaning;
    uniform(1, 1) = meaning;
    uniform(2, 2) = meaning;

    for (unsigned i = 0; i < point.size(); i++) {
      point[i].MulMatrix(uniform);
    }
  }
}

}  // namespace s21
