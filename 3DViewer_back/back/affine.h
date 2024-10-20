#ifndef AFFINE_H
#define AFFINE_H

#include <vector>

#include "../../matrix/s21_matrix_oop.h"

namespace s21 {

class strategy_aff {
 public:
  virtual ~strategy_aff() = default;
  virtual void tf_model(std::vector<S21Matrix> &point, double meaning) = 0;
};

class move_X : public strategy_aff {
 public:
  void tf_model(std::vector<S21Matrix> &point, double meaning) override;
};

class move_Y : public strategy_aff {
 public:
  void tf_model(std::vector<S21Matrix> &point, double meaning) override;
};

class move_Z : public strategy_aff {
 public:
  void tf_model(std::vector<S21Matrix> &point, double meaning) override;
};

class rotate_X : public strategy_aff {
 public:
  void tf_model(std::vector<S21Matrix> &point, double meaning) override;
};

class rotate_Y : public strategy_aff {
 public:
  void tf_model(std::vector<S21Matrix> &point, double meaning) override;
};

class rotate_Z : public strategy_aff {
 public:
  void tf_model(std::vector<S21Matrix> &point, double meaning) override;
};

class scale : public strategy_aff {
 public:
  void tf_model(std::vector<S21Matrix> &point, double meaning) override;
};

class aff_context {
 private:
  strategy_aff *strategy_;

 public:
  aff_context(strategy_aff *strategy = nullptr) : strategy_(strategy){};
  ~aff_context() {
    if (strategy_) {
      delete this->strategy_;
    }
  }

  void set_strategy(strategy_aff *st) {
    if (strategy_) {
      delete this->strategy_;
    }
    this->strategy_ = st;
  }

  void execute_strategy(std::vector<S21Matrix> &arr, double meaning) {
    strategy_->tf_model(arr, meaning);
  }
};

}  // namespace s21

#endif  // AFFINE_H