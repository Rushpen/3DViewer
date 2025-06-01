#include "3dViewer_tests.h"

namespace s21 {

void ViewerTest::SetUp() {
  std::string test_file = "tests/objects/spider.obj";
  controller_.start(test_file);
}

TEST_F(ViewerTest, ApplyControllerTransformMoveY) {
  auto points = controller_.get_point();
  double initial_y = points[0](0, 1);

  controller_.set_status(mv_Y);
  controller_.set_meaning(5.0);
  controller_.transform();

  points = controller_.get_point();
  EXPECT_NEAR(points[0](0, 1), initial_y + 5.0, 1e-6);
}

TEST_F(ViewerTest, ApplyControllerTransformRotateY) {
  auto points = controller_.get_point();
  double initial_x = points[0](0, 0);

  controller_.set_status(rt_Y);
  controller_.set_meaning(180.0);
  controller_.transform();

  points = controller_.get_point();
  EXPECT_NEAR(points[0](0, 0), -1 * initial_x, 1e-6);
}

TEST_F(ViewerTest, ApplyControllerTransformScale1) {
  auto points = controller_.get_point();
  double initial_x = points[0](0, 0);

  controller_.set_status(scal);
  controller_.set_meaning(2);
  controller_.transform();

  points = controller_.get_point();
  EXPECT_NEAR(points[0](0, 0), 2 * initial_x, 1e-6);
}

TEST_F(ViewerTest, ApplyControllerTransformMoveX) {
  controller_.set_status(mv_X);
  controller_.set_meaning(5.0);
  controller_.transform();
  EXPECT_NEAR(-0.113037 + 5.0, -0.113037 + 5.0, 1e-6);
}

TEST_F(ViewerTest, ApplyControllerTransformMoveZ) {
  auto points = controller_.get_point();
  double initial_z = points[0](0, 2);

  controller_.set_status(mv_Z);
  controller_.set_meaning(5.0);
  controller_.transform();

  points = controller_.get_point();
  EXPECT_NEAR(points[0](0, 2), initial_z + 5.0, 1e-6);
}

TEST_F(ViewerTest, ApplyControllerTransformRotateX1) {
  auto points = controller_.get_point();
  double initial_x = points[0](0, 0);

  controller_.set_status(rt_X);
  controller_.set_meaning(180.0);
  controller_.transform();

  points = controller_.get_point();
  EXPECT_EQ(points[0](0, 0), initial_x);
}

TEST_F(ViewerTest, ApplyControllerTransformRotateZ) {
  auto points = controller_.get_point();
  double initial_x = points[0](0, 0);

  controller_.set_status(rt_Z);
  controller_.set_meaning(180.0);
  controller_.transform();

  points = controller_.get_point();
  EXPECT_NEAR(points[0](0, 0), -1 * initial_x, 1e-6);
}

TEST_F(ViewerTest, ApplyControllerTransformScale2) {
  auto points = controller_.get_point();
  double initial_x = points[0](0, 0);
  double initial_y = points[0](0, 1);
  double initial_z = points[0](0, 2);

  controller_.set_status(scal);
  controller_.set_meaning(2);
  controller_.transform();

  points = controller_.get_point();
  EXPECT_NEAR(points[0](0, 0), 2 * initial_x, 1e-6);
  EXPECT_NEAR(points[0](0, 1), 2 * initial_y, 1e-6);
  EXPECT_NEAR(points[0](0, 2), 2 * initial_z, 1e-6);
}

TEST_F(ViewerTest, LoadModelThroughController) {
  auto points = controller_.get_point();
  EXPECT_GT(points.size(), 0);
}

TEST_F(ViewerTest, ApplyControllerOther) {
  std::set<segment> test_face = controller_.get_face();
  EXPECT_EQ(test_face, controller_.get_face());
  segment test_segment = *test_face.begin();
  EXPECT_EQ(*test_face.begin(), test_segment);
}

TEST_F(ViewerTest, ApplyControllerVertixesSize) {
  unsigned vert_size = controller_.get_number_vertices();
  EXPECT_NE(vert_size, 0);
}

TEST_F(ViewerTest, ApplyControllerEdgesSize) {
  unsigned edge_size = controller_.get_number_edges();
  EXPECT_NE(edge_size, 0);
}

TEST_F(ViewerTest, ApplyControllerNonFile) {
  std::string test_file = "1.obj";
  controller_.start(test_file);

  std::string test_file1 = "tests/objects/2.obj";
  controller_.start(test_file1);
}
}  // namespace s21