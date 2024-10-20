#include "controller.h"

namespace s21 {

void Controller::start(const std::string &file_name) {
  model_general.set_model(file_name);
}

void Controller::transform() {
  model_general.state_model(status, model_general.get_point(), meaning);
}

}  // namespace s21
