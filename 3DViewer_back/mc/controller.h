#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "model.h"
#include <chrono>
#include <iostream>

namespace s21 {

class Controller {
 private:
  Model model_general;
  State status;
  double meaning;

 public:
  Controller() = default;
  ~Controller() = default;

  void start(const std::string &file_name);

  void set_status(State st) { status = st; }

  void set_meaning(double mean) { meaning = mean; }

  std::set<segment> get_face() { return model_general.get_face(); }

  std::vector<S21Matrix> &get_point() { return model_general.get_point(); }

  unsigned get_number_vertices() { return model_general.get_point().size(); }

  unsigned get_number_edges() { return model_general.get_face().size(); }

  void transform();

  void print() { model_general.print(); }
};

}  // namespace s21

#endif  // CONTROLLER_H
