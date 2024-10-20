#include <vector>

#include "gtest/gtest.h"
#include "tests.h"

namespace s21 {

class ViewerTest : public ::testing::Test {
 protected:
  Controller controller_;

  void SetUp() override;
};
}  // namespace s21
