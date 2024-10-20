#include "s21_matrix_oop.h"

S21Matrix::S21Matrix() : S21Matrix(5, 5) {}

S21Matrix::S21Matrix(int rows, int cols) {
  if (rows < 1 || cols < 1) {
    throw out_of_range("Matriced should have same size");
  } else {
    rows_ = rows;
    cols_ = cols;
    matrix_ = new double*[rows_];
    for (int i = 0; i < rows_; i++) {
      matrix_[i] = new double[cols_];
      for (int j = 0; j < cols_; j++) {
        matrix_[i][j] = 0.0;
      }
    }
  }
}

S21Matrix::S21Matrix(const S21Matrix& other) {
  rows_ = other.rows_;
  cols_ = other.cols_;
  matrix_ = new double*[rows_];
  for (int i = 0; i < rows_; i++) {
    matrix_[i] = new double[cols_];
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] = other.matrix_[i][j];
    }
  }
}

S21Matrix::S21Matrix(S21Matrix&& other) {
  rows_ = other.rows_;
  cols_ = other.cols_;
  matrix_ = other.matrix_;
  other.matrix_ = nullptr;
  other.rows_ = 0;
  other.cols_ = 0;
}

S21Matrix::~S21Matrix() {
  if (matrix_ != nullptr) {
    for (int i = 0; i < rows_; i++) {
      delete[] matrix_[i];
    }
    delete[] matrix_;
  }
  matrix_ = nullptr;
  rows_ = 0;
  cols_ = 0;
}

//////////// accessor and mutator
int S21Matrix::get_rows_() const { return rows_; }

int S21Matrix::get_cols_() const { return cols_; }

void S21Matrix::set_rows_(int rows) {
  if (rows < 1) {
    throw out_of_range("Matriced should have same size");
  } else {
    S21Matrix tmp(rows, cols_);
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols_; j++) {
        tmp.matrix_[i][j] = i < rows_ ? matrix_[i][j] : 0.0;
      }
    }
    *this = tmp;
  }
}

void S21Matrix::set_cols_(int cols) {
  if (cols < 1) {
    throw out_of_range("Matriced should have same size");
  } else {
    S21Matrix tmp(rows_, cols);
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols; j++) {
        tmp.matrix_[i][j] = j < cols_ ? matrix_[i][j] : 0.0;
      }
    }
    *this = tmp;
  }
}

//////////// operator
S21Matrix S21Matrix::operator=(const S21Matrix& other) noexcept {
  this->~S21Matrix();
  rows_ = other.rows_;
  cols_ = other.cols_;
  matrix_ = new double*[rows_];
  for (int i = 0; i < rows_; i++) {
    matrix_[i] = new double[cols_];
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] = other.matrix_[i][j];
    }
  }

  return *this;
}

S21Matrix S21Matrix::operator+(const S21Matrix& other) {
  S21Matrix result(rows_, cols_);
  result = *this;
  result.SumMatrix(other);
  return result;
}

S21Matrix S21Matrix::operator-(const S21Matrix& other) {
  S21Matrix result(rows_, cols_);
  result = *this;
  result.SubMatrix(other);
  return result;
}

S21Matrix S21Matrix::operator*(double num) {
  S21Matrix result(rows_, cols_);
  result = *this;
  result.MulNumber(num);
  return result;
}

S21Matrix S21Matrix::operator*(const S21Matrix& other) {
  S21Matrix result(rows_, cols_);
  result = *this;
  result.MulMatrix(other);
  return result;
}

bool S21Matrix::operator==(const S21Matrix& other) {
  return this->EqMatrix(other);
}

S21Matrix S21Matrix::operator+=(const S21Matrix& other) {
  this->SumMatrix(other);
  return *this;
}

S21Matrix S21Matrix::operator-=(const S21Matrix& other) {
  this->SubMatrix(other);
  return *this;
}

S21Matrix S21Matrix::operator*=(const S21Matrix& other) {
  this->MulMatrix(other);
  return *this;
}

double& S21Matrix::operator()(int row, int col) {
  if (row < 0 || row >= rows_ || col < 0 || col >= cols_) {
    throw out_of_range("Incorrect input, index is out of range");
  }
  return matrix_[row][col];
}

double& S21Matrix::operator()(int row, int col) const {
  if (row < 0 || row >= rows_ || col < 0 || col >= cols_) {
    throw out_of_range("Incorrect input, index is out of range");
  }
  return matrix_[row][col];
}

void S21Matrix::set_el_Matrix(int row, int col, double result) {
  matrix_[row][col] = result;
}

// methods
bool S21Matrix::EqMatrix(const S21Matrix& other) {
  bool result = true;

  if (rows_ != other.rows_ || cols_ != other.cols_) {
    result = false;
  } else if (matrix_ == nullptr || other.matrix_ == nullptr) {
    result = false;
  } else {
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols_; j++) {
        if ((fabs(matrix_[i][j] - other.matrix_[i][j])) > EPS) {
          result = false;
        }
      }
    }
  }
  return result;
}

void S21Matrix::SumMatrix(const S21Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    throw out_of_range("Matriced should have same size");
  } else if (matrix_ == nullptr || other.matrix_ == nullptr) {
    throw length_error("Matrices nullptr");
  } else {
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols_; j++) {
        matrix_[i][j] += other.matrix_[i][j];
      }
    }
  }
}

void S21Matrix::SubMatrix(const S21Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    throw out_of_range("Matrices should have same equal size");
  } else if (matrix_ == nullptr || other.matrix_ == nullptr) {
    throw length_error("Matrices nullptr");
  } else {
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols_; j++) {
        matrix_[i][j] -= other.matrix_[i][j];
      }
    }
  }
}

void S21Matrix::MulNumber(const double num) {
  if (rows_ < 1 || cols_ < 1) {
    throw out_of_range("Matrices should have same equal size");
  } else if (matrix_ == nullptr) {
    throw length_error("Matrices nullptr");
  } else {
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols_; j++) {
        matrix_[i][j] *= num;
      }
    }
  }
}

void S21Matrix::MulMatrix(const S21Matrix& other) {
  if (cols_ != other.rows_) {
    throw out_of_range(
        "Number of columns first matrix is not equal to number of rows second "
        "matrix.");
  } else if (matrix_ == nullptr || other.matrix_ == nullptr) {
    throw length_error("Matrices nullptr");
  } else {
    S21Matrix tmp(rows_, other.cols_);
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < other.cols_; j++) {
        for (int k = 0; k < cols_; k++) {
          tmp.matrix_[i][j] += matrix_[i][k] * other.matrix_[k][j];
        }
      }
    }
    *this = tmp;
  }
}

S21Matrix S21Matrix::Transpose() {
  if (rows_ < 1 || cols_ < 1) {
    throw out_of_range("Matrices should have same equal size");
  } else if (matrix_ == nullptr) {
    throw length_error("Matrices nullptr");
  } else {
    S21Matrix tmp(cols_, rows_);
    for (int i = 0; i < cols_; i++) {
      for (int j = 0; j < rows_; j++) {
        tmp.matrix_[i][j] = matrix_[j][i];
      }
    }
    return tmp;
  }
}

S21Matrix S21Matrix::MinorMat(int number_del_row, int number_del_col) {
  S21Matrix minor(rows_ - 1, cols_ - 1);
  int minor_i = 0;
  for (int i = 0; i < rows_; i++) {
    if (i != number_del_row) {
      int minor_j = 0;
      for (int j = 0; j < cols_; j++) {
        if (j != number_del_col) {
          minor.matrix_[minor_i][minor_j] = matrix_[i][j];
          minor_j++;
        }
      }
      minor_i++;
    }
  }
  return minor;
}

S21Matrix S21Matrix::CalcComplements() {
  if (rows_ < 1 || cols_ < 1) {
    throw out_of_range("Matrices should have same equal size");
  } else if (matrix_ == nullptr) {
    throw length_error("Matrices nullptr");
  } else if (rows_ != cols_) {
    throw length_error("Rows != Colums!!!");
  } else {
    S21Matrix calc(rows_, cols_);
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols_; j++) {
        S21Matrix tmp;
        tmp = this->MinorMat(i, j);
        calc.matrix_[i][j] = tmp.Determinant() * pow(-1, i + j);
      }
    }
    return calc;
  }
}

double S21Matrix::Determinant() {
  if (rows_ < 1 || cols_ < 1) {
    throw out_of_range("Matrices should have same equal size");
  } else if (matrix_ == nullptr) {
    throw length_error("Matrices nullptr");
  } else if (rows_ != cols_) {
    throw length_error("Rows != Colums!!!");
  } else {
    int result = 0.0;
    if (rows_ == 1) {
      result = matrix_[0][0];
    } else if (rows_ == 2) {
      result = matrix_[0][0] * matrix_[1][1] - matrix_[0][1] * matrix_[1][0];
    } else {
      for (int i = 0; i < rows_; i++) {
        S21Matrix tmp;
        tmp = this->MinorMat(0, i);
        result += pow(-1, i) * matrix_[0][i] * tmp.Determinant();
      }
    }
    return result;
  }
}

S21Matrix S21Matrix::InverseMatrix() {
  if (rows_ < 1 || cols_ < 1) {
    throw out_of_range("Matrices should have same equal size");
  } else if (matrix_ == nullptr) {
    throw length_error("Matrices nullptr");
  } else {
    if (fabs(this->Determinant()) < EPS) {
      throw length_error("Determinant == 0!!!");
    } else {
      S21Matrix calc;
      calc = this->CalcComplements();

      S21Matrix trans;
      trans = calc.Transpose();

      trans.MulNumber(this->Determinant());
      return trans;
    }
  }
}
