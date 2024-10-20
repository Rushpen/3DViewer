cmake_minimum_required(VERSION 3.10)

project(s21_3DViewer_v2.0_Test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_BUILD_TYPE Release)

if(CMAKE_BUILD_TYPE EQUAL "Debug")
  add_compile_options(-fsanitize=address)
  add_link_options(-fsanitize=address)
endif()

add_library(s21_3DViewer_v2.0 STATIC ../3DViewer_back/back/affine.cpp ../3DViewer_back/back/parser.cpp ../3DViewer_back/mc/controller.cpp ../3DViewer_back/mc/model.cpp ../matrix/s21_matrix_oop.cpp)
target_compile_options(s21_3DViewer_v2.0 PRIVATE -Wall -Werror -Wextra -Wpedantic)

find_package(GTest REQUIRED)
include(GoogleTest)
enable_testing()


file(GLOB SOURCES ./tests.cpp ./3dViewer_tests.cpp)
add_executable(tests ${SOURCES})
target_compile_options(tests PRIVATE -Wall -Werror -Wextra -Wpedantic -g)

target_link_libraries(tests GTest::gtest_main s21_3DViewer_v2.0)

gtest_discover_tests(tests)
