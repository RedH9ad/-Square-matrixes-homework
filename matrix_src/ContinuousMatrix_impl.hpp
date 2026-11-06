#pragma once
#include "CommonDeterminantCountMixin.hpp"
#include "ContinuousMatrix.hpp"
#include <cstddef>
#include <stdexcept>
#include <vector>

namespace QuadraticMatrix {
template <class ValType>
ContinuousMatrix<ValType>::ContinuousMatrix(size_t size) : size_(size) {
  matrix_ = new ValType[size_ * size_]{};
}

template <class ValType>
template <class SeqIt>
ContinuousMatrix<ValType>::ContinuousMatrix(size_t size, SeqIt begin, SeqIt end)
    : ContinuousMatrix(size) {
  for (size_t i = 0; i < size_ * size_; i++) {
    if (begin == end) {
      throw std::invalid_argument(
          "Размер буфера не достаточен для создания матрицы");
    }
    matrix_[i] = *begin;
    begin++;
  }
  if (begin != end) {
    throw std::invalid_argument("Размер буфера избыточен для создания матрицы");
  }
}

template <class ValType>
ContinuousMatrix<ValType>::ContinuousMatrix(
    const std::vector<std::vector<ValType>> &matrix)
    : ContinuousMatrix(matrix.size()) {
  for (size_t row = 0; row < size_; row++) {
    if (matrix[row].size() != size_) {
      throw std::runtime_error("Попытка создать квадратную матрицу из "
                               "двумерного std::vector не квадратных размеров");
    }
    for (size_t col = 0; col < size_; col++) {
      (*this)[row][col] = matrix[row][col];
    }
  }
}

template <class ValType>
ContinuousMatrix<ValType>::ContinuousMatrix(const ContinuousMatrix &rhs)
    : ContinuousMatrix(rhs.size_) {
  std::copy(rhs.matrix_, rhs.matrix_ + rhs.size_ * rhs.size_, matrix_);
}

template <class ValType>
ContinuousMatrix<ValType>::ContinuousMatrix(ContinuousMatrix &&rhs) noexcept
    : matrix_(rhs.matrix_), size_(rhs.size_) {
  rhs.matrix_ = nullptr;
  rhs.size_ = 0;
}

template <class ValType>
ContinuousMatrix<ValType> &
ContinuousMatrix<ValType>::operator=(const ContinuousMatrix &other) {
  if (&other == this) {
    return *this;
  }
  ContinuousMatrix temp(other);
  std::swap(matrix_, temp.matrix_);
  std::swap(size_, temp.size_);
  return *this;
}

template <class ValType>
ContinuousMatrix<ValType> &
ContinuousMatrix<ValType>::operator=(ContinuousMatrix &&other) noexcept {
  if (&other == this) {
    return *this;
  }
  std::swap(matrix_, other.matrix_);
  std::swap(size_, other.size_);
  return *this;
}

template <class ValType> ContinuousMatrix<ValType>::~ContinuousMatrix() {
  delete[] matrix_;
}

template <class ValType>
ContinuousMatrix<ValType>::ProxyRow
ContinuousMatrix<ValType>::operator[](size_t row) {
  return ProxyRow(matrix_ + size_ * row);
}

template <class ValType>
ContinuousMatrix<ValType>::ConstProxyRow
ContinuousMatrix<ValType>::operator[](size_t row) const {
  return ConstProxyRow(matrix_ + size_ * row);
}

template <class ValType> size_t ContinuousMatrix<ValType>::GetSize() const {
  return size_;
}

template <class ValType>
ContinuousMatrix<ValType> ContinuousMatrix<ValType>::Eye(size_t size) {
  ContinuousMatrix res(size);
  for (size_t i = 0; i < size; i++) {
    res[i][i] = 1;
  }
  return res;
}

template <class ValType>
void ContinuousMatrix<ValType>::SwapTwoRows(size_t first, size_t second) {
  ValType *row1 = matrix_ + first * size_;
  ValType *row2 = matrix_ + second * size_;
  std::swap_ranges(row1, row1 + size_, row2);
}

template <class ValType>
ContinuousMatrix<ValType> ContinuousMatrix<ValType>::Transpose() const {
  ContinuousMatrix temp(size_);
  for (size_t i = 0; i < size_; i++) {
    for (size_t j = 0; j < size_; j++) {
      temp[i][j] = (*this)[j][i];
    }
  }
  return temp;
}

template <class ValType>
bool ContinuousMatrix<ValType>::Equal(const ContinuousMatrix<ValType> &other,
                                      ValType eps) const {
  if (size_ != other.GetSize()) {
    return false;
  }
  for (size_t row = 0; row < size_; row++) {
    for (size_t col = 0; col < size_; col++) {
      if (std::abs((*this)[row][col] - other[row][col]) > eps) {
        return false;
      }
    }
  }
  return true;
}

template <class ValType>
ContinuousMatrix<ValType> ContinuousMatrix<ValType>::operator*(
    const ContinuousMatrix<ValType> &other) const {
  if (other.GetSize() != size_) {
    throw std::runtime_error("Попытка перемножить матрицы разного размера");
  }

  ContinuousMatrix temp(size_);
  for (size_t row = 0; row < size_; row++) {
    for (size_t iter = 0; iter < size_; iter++) {
      auto multiplyer = *(matrix_ + row * size_ + iter);

      for (size_t col = 0; col < size_; col++) {
        (*(temp.matrix_ + row * size_ + col)) +=
            multiplyer * (*(other.matrix_ + iter * size_ + col));
      }
    }
  }
  return temp;
}

template <class ValType>
ContinuousMatrix<ValType> ContinuousMatrix<ValType>::operator+(
    const ContinuousMatrix<ValType> &other) const {
  if (other.GetSize() != size_) {
    throw std::runtime_error("Попытка сложить матрицы разного размера");
  }

  ContinuousMatrix<ValType> temp(*this);
  size_t arr_size = size_ * size_;
  for (size_t iter = 0; iter < arr_size; iter++) {
    temp.matrix_[iter] += other.matrix_[iter];
  }
  return temp;
}
} // namespace QuadraticMatrix
