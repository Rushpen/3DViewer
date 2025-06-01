#include "controller.h"

namespace s21 {

void Controller::start(const std::string &file_name) {
  auto start_time = std::chrono::high_resolution_clock::now();
  model_general.set_model(file_name);
  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
  std::cout << "Model loaded in " << duration.count() << " ms" << std::endl;
}

void Controller::transform() {
  model_general.state_model(status, model_general.get_point(), meaning);
}

}  // namespace s21
