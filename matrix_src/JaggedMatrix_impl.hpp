#pragma once
#include "CommonDeterminantCountMixin.hpp"
#include "JaggedMatrix.hpp"
#include <algorithm>
#include <concepts>
#include <csignal>
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <utility>

namespace QuadraticMatrix {

template <class ValType>
JaggedMatrix<ValType>::JaggedMatrix(size_t size) : size_(size) {
  matrix_ = new ValType *[size_];
  for (size_t i = 0; i < size_; i++) {
    matrix_[i] = new ValType[size_]{};
  }
}

template <class ValType>
JaggedMatrix<ValType>::JaggedMatrix(size_t size, ValType value)
    : JaggedMatrix(size) {
  for (size_t i = 0; i < size_; i++) {
    std::fill(matrix_[i], matrix_[i] + size_, value);
  }
}

template <class ValType>
template <class SeqIt>
JaggedMatrix<ValType>::JaggedMatrix(size_t size, SeqIt begin, SeqIt end)
    : JaggedMatrix(size) {
  for (size_t row = 0; row < size_; row++) {
    for (size_t col = 0; col < size_; col++) {
      if (begin == end) {
        throw std::invalid_argument(
            "Размер буфера не достаточен для создания матрицы");
      }
      matrix_[row][col] = *begin;
      begin++;
    }
  }
  if (begin != end) {
    throw std::invalid_argument("Размер буфера избыточен для создания матрицы");
  }
}

template <class ValType>
JaggedMatrix<ValType>::JaggedMatrix(
    const std::vector<std::vector<ValType>> &matrix)
    : JaggedMatrix(matrix.size()) {
  if (matrix[0].size() != matrix.size()) {
    throw std::runtime_error("Попытка создать квадратную матрицу из двумерного "
                             "std::vector не квадратных размеров");
  }
  for (size_t row = 0; row < size_; row++) {
    std::copy(matrix[row].begin(), matrix[row].end(), matrix_[row]);
  }
}

template <class ValType>
JaggedMatrix<ValType>::JaggedMatrix(const JaggedMatrix<ValType> &rhs)
    : JaggedMatrix(rhs.size_) {
  if (rhs.matrix_) {
    for (size_t i = 0; i < size_; i++) {
      std::copy(rhs.matrix_[i], rhs.matrix_[i] + size_, matrix_[i]);
    }
  }
}

template <class ValType>
JaggedMatrix<ValType>::JaggedMatrix(JaggedMatrix<ValType> &&rhs) noexcept
    : matrix_(rhs.matrix_), size_(rhs.size_) {
  rhs.matrix_ = nullptr;
  rhs.size_ = 0;
}

template <class ValType>
JaggedMatrix<ValType> &
JaggedMatrix<ValType>::operator=(const JaggedMatrix<ValType> &other) {
  if (&other == this) {
    return *this;
  }
  JaggedMatrix temp(other);
  std::swap(matrix_, temp.matrix_);
  std::swap(size_, temp.size_);
  return *this;
}

template <class ValType>
JaggedMatrix<ValType> &
JaggedMatrix<ValType>::operator=(JaggedMatrix<ValType> &&other) noexcept {
  if (&other == this) {
    return *this;
  }
  std::swap(matrix_, other.matrix_);
  std::swap(size_, other.size_);
  return *this;
}

template <class ValType> JaggedMatrix<ValType>::~JaggedMatrix() {
  if (matrix_) {
    for (size_t i = 0; i < size_; i++) {
      delete[] matrix_[i];
    }
    delete[] matrix_;
  }
}

template <class ValType>
JaggedMatrix<ValType> JaggedMatrix<ValType>::Eye(size_t size) {
  auto res = JaggedMatrix(size);
  for (size_t i = 0; i < res.size_; i++) {
    res[i][i] = static_cast<ValType>(1);
  }
  return res;
}

template <class ValType>
JaggedMatrix<ValType> JaggedMatrix<ValType>::Transpose() const {
  auto res = JaggedMatrix(size_);
  for (size_t i = 0; i < size_; i++) {
    for (size_t j = 0; j < size_; j++) {
      res[i][j] = matrix_[j][i];
    }
  }
  return res;
}

template <class ValType> size_t JaggedMatrix<ValType>::GetSize() const {
  return size_;
}

template <class ValType>
void JaggedMatrix<ValType>::SwapTwoRows(size_t first, size_t second) {
  std::swap(matrix_[first], matrix_[second]);
}

template <class ValType>
bool JaggedMatrix<ValType>::Equal(const JaggedMatrix<ValType> &other,
                                  ValType eps) const {
  if (size_ != other.GetSize()) {
    return false;
  }
  for (size_t row = 0; row < size_; row++) {
    for (size_t col = 0; col < size_; col++) {
      if (std::abs((*this)[col][row] - other[col][row]) > eps) {
        return false;
      }
    }
  }
  return true;
}

template <class ValType>
JaggedMatrix<ValType>
JaggedMatrix<ValType>::operator*(const JaggedMatrix<ValType> &other) const {
  if (other.GetSize() != size_) {
    throw std::runtime_error("Попытка перемножить матрицы разного размера");
  }

  JaggedMatrix temp(size_);
  for (size_t row = 0; row < size_; row++) {
    for (size_t iter = 0; iter < size_; iter++) {
      auto multiplyer = (*this)[row][iter];

      for (size_t col = 0; col < size_; col++) {
        temp[row][col] += multiplyer * other[iter][col];
      }
    }
  }
  return temp;
}

template <class ValType>
JaggedMatrix<ValType>
JaggedMatrix<ValType>::operator+(const JaggedMatrix<ValType> &other) const {
  if (other.GetSize() != size_) {
    throw std::runtime_error("Попытка сложить матрицы разного размера");
  }

  JaggedMatrix<ValType> temp(*this);
  for (size_t row = 0; row < size_; row++) {
    for (size_t col = 0; col < size_; col++) {
      temp[row][col] += other[row][col];
    }
  }
  return temp;
}
} // namespace QuadraticMatrix
