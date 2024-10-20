#ifndef MODEL_H
#define MODEL_H

#include "../back/affine.h"
#include "../back/parser.h"

namespace s21 {

enum State { mv_X = 0, mv_Y, mv_Z, rt_X, rt_Y, rt_Z, scal, start };

class Model {
 private:
  aff_context aff_strategy;
  parser *parser_;

 public:
  Model() = default;
  ~Model() = default;

  void set_model(const std::string &file_name);

  std::vector<S21Matrix> &get_point() { return parser_->get_point(); }

  std::set<segment> get_face() const { return parser_->get_face(); }

  void state_model(State st, std::vector<S21Matrix> &point, double meaning);

  void print() { parser_->print_data(); }
};

}  // namespace s21

#endif  // MODEL_H
