#ifndef PARSER_H
#define PARSER_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "../../matrix/s21_matrix_oop.h"

namespace s21 {
class segment {
 private:
  unsigned first_;
  unsigned second_;

 public:
  segment(unsigned first, unsigned second) : first_(first), second_(second){};
  ~segment() = default;

  unsigned get_segment_first() const;
  unsigned get_segment_second() const;
  void sort_segment();

  bool operator==(const segment &other) const;
  bool operator<(const segment &other) const;
};

class parser {
 public:
  static parser *get_singleton();
  std::vector<S21Matrix> &get_point();
  std::set<segment> get_face() const;
  int open_file(const std::string &file_name);
  void delete_data();

  void print_data();

 protected:
  parser(parser &other) = delete;
  void operator=(const parser &) = delete;
  parser() = default;
  ~parser() {
    if (!point_.empty()) {
      point_.clear();
    }
    if (!face_.empty()) {
      face_.clear();
    }
    if (singleton_) {
      delete singleton_;
    }
  }

 private:
  int selection_parser(const std::string &current_line);

  int pr_point(const std::string &current_line);
  int pr_face(const std::string &current_line);

  static parser *singleton_;

  std::vector<S21Matrix> point_;
  std::set<segment> face_;
};

}  // namespace s21

#endif  // PARSER_H
