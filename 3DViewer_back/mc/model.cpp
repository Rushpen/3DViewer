#include "model.h"

namespace s21 {

void Model::set_model(const std::string &file_name) {
  parser_ = s21::parser::get_singleton();
  parser_->open_file(file_name);
}

void Model::state_model(State st, std::vector<S21Matrix> &point,
                        double meaning) {
  if (st == mv_X) {
    aff_strategy.set_strategy(new move_X());
  } else if (st == mv_Y) {
    aff_strategy.set_strategy(new move_Y());
  } else if (st == mv_Z) {
    aff_strategy.set_strategy(new move_Z());
  } else if (st == rt_X) {
    aff_strategy.set_strategy(new rotate_X());
  } else if (st == rt_Y) {
    aff_strategy.set_strategy(new rotate_Y());
  } else if (st == rt_Z) {
    aff_strategy.set_strategy(new rotate_Z());
  } else if (st == scal) {
    aff_strategy.set_strategy(new scale());
  }
  aff_strategy.execute_strategy(point, meaning);
}

}  // namespace s21
