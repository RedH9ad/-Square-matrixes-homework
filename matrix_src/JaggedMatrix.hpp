#pragma once
#include "CommonDeterminantCountMixin.hpp"
#include <concepts>
#include <cstddef>
#include <iostream>
#include <vector>

namespace QuadraticMatrix {
template <class ValType>
class JaggedMatrix : public DeterminantMixin<JaggedMatrix<ValType>, ValType> {
private:
  friend class DeterminantMixin<JaggedMatrix<ValType>, ValType>;
  ValType **matrix_ = nullptr;
  size_t size_ = 0;

  // Приватные вспомогательные методы для нахождения детерминанта матрицы чисел
  // с плавающей точкой
  void SwapTwoRows(size_t first, size_t second);

public:
  // По дефолту инициализируется нулевой матрицей заданного размера
  JaggedMatrix() = default;
  JaggedMatrix(size_t size);

  // Конструктор на случай когда хотим создать матрицу заполненную значениями
  JaggedMatrix(size_t size, ValType value);

  // Конструктор для создания из заданной плоской последовательности
  template <class SeqIt> JaggedMatrix(size_t size, SeqIt begin, SeqIt end);
  explicit JaggedMatrix(const std::vector<std::vector<ValType>> &matrix);
  // Создание единичной матрицы размера Size
  static JaggedMatrix Eye(size_t size);

  JaggedMatrix(const JaggedMatrix &rhs);
  JaggedMatrix(JaggedMatrix &&rhs) noexcept;
  JaggedMatrix &operator=(const JaggedMatrix &other);
  JaggedMatrix &operator=(JaggedMatrix &&other) noexcept;

  ValType *operator[](size_t row) { return matrix_[row]; }
  const ValType *operator[](size_t row) const { return matrix_[row]; }

  JaggedMatrix Transpose() const;
  bool Equal(const JaggedMatrix &other,
             ValType eps = static_cast<ValType>(1e-10)) const;
  size_t GetSize() const;

  JaggedMatrix operator*(const JaggedMatrix &other) const;
  JaggedMatrix operator+(const JaggedMatrix &other) const;

  ~JaggedMatrix();
};
} // namespace QuadraticMatrix

#include "JaggedMatrix_impl.hpp"