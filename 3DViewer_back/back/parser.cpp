#include "parser.h"

namespace s21 {

parser *parser::singleton_ = nullptr;

parser *parser::get_singleton() {
  if (singleton_ == nullptr) {
    singleton_ = new parser();
  }
  return singleton_;
}

std::vector<S21Matrix> &parser::get_point() { return point_; }

std::set<segment> parser::get_face() const { return face_; }

int parser::open_file(const std::string &file_name) {
  int error = 0;
  delete_data();
  std::ifstream file(file_name);
  if (file.is_open()) {
    std::string current_line;
    while (
        std::getline(file, current_line)) {
      int lineError = selection_parser(
          current_line);

      if (lineError != 0) error = lineError;
    }

    file.close();

  } else {
    error = 1;
  }
  return error;
}

void parser::delete_data() {
  if (!point_.empty()) {
    point_.clear();
  }
  if (!face_.empty()) {
    face_.clear();
  }
};

int parser::selection_parser(const std::string &current_line) {
  int error = 0;

  if (current_line.empty()) return error;

  if (current_line[0] == 'v' && current_line[1] == ' ') {
    error = pr_point(current_line);
  } else if (current_line[0] == 'f' && current_line[1] == ' ') {
    error = pr_face(current_line);
  }

  return error;
}

int parser::pr_point(const std::string &current_line) {
  int error = 0;
  std::istringstream line(current_line);
  double x, y, z;
  if (line.ignore(1) >> x >> y >> z) {
    S21Matrix point(1, 3);
    point(0, 0) = x;
    point(0, 1) = y;
    point(0, 2) = z;
    point_.push_back(point);
  } else if (line.eof()) {
    error = 0;
  } else {
    error = 1;
  }
  return error;
}

int parser::pr_face(const std::string &current_line) {
  int error = 0;
  std::string cleaned_line;
  bool skip = false;

  for (char c : current_line) {
    if (c == '/') {
      skip = true;
    } else if (c == ' ') {
      skip = false;
      cleaned_line += c;
    } else if (!skip) {
      cleaned_line += c;
    }
  }

  std::istringstream line(cleaned_line);

  unsigned first;
  unsigned second;

  line.ignore(1);

  line >> first;

  while (line >> second) {
    segment vector(first, second);
    vector.sort_segment();
    first = second;
    face_.insert(vector);
  }

  if (line.eof()) {
    error = 0;
  } else {
    error = 1;
  }

  return error;
}

void parser::print_data() {
  int i = 0;
  std::cout << "File parsed successfully!\n";
  std::cout << "Points:\n";
  for (const auto &point : point_) {
    i++;
    std::cout << "v " << point(0, 0) << " " << point(0, 1) << " " << point(0, 2)
              << "    " << i << "\n";
  }
  i = 0;
  std::cout << "Faces:\n";
  for (const auto &face : face_) {
    i++;
    std::cout << "f ";
    std::cout << face.get_segment_first() << " " << face.get_segment_second()
              << "    " << i << std::endl;
  }
  std::cout << std::endl;
}

unsigned segment::get_segment_first() const { return first_; }

unsigned segment::get_segment_second() const { return second_; }

void segment::sort_segment() {
  if (first_ > second_) {
    unsigned tmp = first_;
    first_ = second_;
    second_ = tmp;
  }
}

bool segment::operator==(const segment &other) const {
  bool result = false;
  if (first_ == other.first_) {
    if (second_ == other.second_) {
      result = true;
    }
  }
  return result;
}

bool segment::operator<(const segment &other) const {
  if (first_ < other.first_) {
    return true;
  } else if (first_ == other.first_) {
    if (second_ < other.second_) {
      return true;
    }
  }
  return false;
}

}  // namespace s21
