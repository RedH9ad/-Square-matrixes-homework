#pragma once
#include "CommonDeterminantCountMixin.hpp"
#include <cstddef>
#include <iostream>
#include <vector>

namespace QuadraticMatrix {
template <class ValType>
class ContinuousMatrix
    : public DeterminantMixin<ContinuousMatrix<ValType>, ValType> {
private:
  ValType *matrix_ = nullptr;
  size_t size_ = 0;
  friend class DeterminantMixin<ContinuousMatrix<ValType>, ValType>;

  void SwapTwoRows(size_t first, size_t second);

public:
  class ProxyRow {
    ValType *row_begin_;

  public:
    explicit ProxyRow(ValType *row_begin) : row_begin_(row_begin) {}
    ValType &operator[](size_t col) { return *(row_begin_ + col); }
  };

  class ConstProxyRow {
    const ValType *row_begin_;

  public:
    explicit ConstProxyRow(const ValType *row_begin) : row_begin_(row_begin) {}
    const ValType &operator[](size_t col) const { return *(row_begin_ + col); }
  };

  ContinuousMatrix() = default;
  ContinuousMatrix(size_t size);
  template <class SeqIt> ContinuousMatrix(size_t size, SeqIt begin, SeqIt end);
  explicit ContinuousMatrix(const std::vector<std::vector<ValType>> &matrix);
  static ContinuousMatrix Eye(size_t size);

  ContinuousMatrix(const ContinuousMatrix &rhs);
  ContinuousMatrix(ContinuousMatrix &&rhs) noexcept;
  ContinuousMatrix &operator=(const ContinuousMatrix &other);
  ContinuousMatrix &operator=(ContinuousMatrix &&other) noexcept;
  ~ContinuousMatrix();

  ProxyRow operator[](size_t row);
  ConstProxyRow operator[](size_t row) const;

  ContinuousMatrix Transpose() const;
  bool Equal(const ContinuousMatrix &other,
             ValType eps = static_cast<ValType>(1e-10)) const;
  size_t GetSize() const;

  ContinuousMatrix operator*(const ContinuousMatrix &other) const;
  ContinuousMatrix operator+(const ContinuousMatrix &other) const;
};
} // namespace QuadraticMatrix

#include "ContinuousMatrix_impl.hpp"