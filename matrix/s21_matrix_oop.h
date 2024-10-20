#ifndef S21_MATRIX_OPP_H
#define S21_MATRIX_OPP_H

#define EPS 1e-7

#include <cmath>
#include <stdexcept>
using namespace std;

class S21Matrix {
 private:
  int rows_, cols_;
  double** matrix_;

 public:
  S21Matrix operator=(const S21Matrix& other) noexcept;
  S21Matrix(int rows, int cols);
  S21Matrix();

  S21Matrix(const S21Matrix& other);
  S21Matrix(S21Matrix&& other);
  ~S21Matrix();

  int get_rows_() const;
  int get_cols_() const;
  void set_rows_(int rows);
  void set_cols_(int cols);

  S21Matrix operator+(const S21Matrix& other);
  S21Matrix operator-(const S21Matrix& other);
  S21Matrix operator*(double num);
  S21Matrix operator*(const S21Matrix& other);
  bool operator==(const S21Matrix& other);

  S21Matrix operator+=(const S21Matrix& other);
  S21Matrix operator-=(const S21Matrix& other);
  S21Matrix operator*=(const S21Matrix& other);

  double& operator()(int row, int col);
  double& operator()(int row, int col) const;

  void set_el_Matrix(int row, int col, double result);

  bool EqMatrix(const S21Matrix& other);
  void SumMatrix(const S21Matrix& other);
  void SubMatrix(const S21Matrix& other);
  void MulNumber(const double num);
  void MulMatrix(const S21Matrix& other);
  S21Matrix Transpose();
  S21Matrix CalcComplements();
  double Determinant();
  S21Matrix InverseMatrix();

  S21Matrix MinorMat(int number_del_row, int number_del_col);
};

#endif
