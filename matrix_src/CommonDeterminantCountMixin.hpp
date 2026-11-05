#pragma once
#include <cstddef>
#include <iostream>

namespace QuadraticMatrix {
template <class BaseMatrix, class ValType> class DeterminantMixin {
  BaseMatrix *matrix() { return static_cast<BaseMatrix *>(this); }
  const BaseMatrix *matrix() const {
    return static_cast<const BaseMatrix *>(this);
  }
  void AddMultipliedRows(size_t id_from, size_t id_to,
                         ValType multiplier = static_cast<ValType>(1));

  void ToTriangular(bool &row_change, bool &zero_column, ValType eps);

  // Меняет текущую строку на строку с максимальным по модулю элементом для
  // повышения численной устойчивости
  size_t FindMaxPivot(bool &zero_column, size_t iteration, ValType eps);

  void ProcessConsistenIterationGauss(size_t iteration);

  void ProcessZeroMinor(size_t iteration, bool &row_change, bool &zero_column);
  void ProcessConsistenIterationBareiss(size_t iteration,
                                        ValType previous_minor);

  void OneInvertionIteration(size_t iteration, ValType eps, BaseMatrix &res);

public:
  ValType CountDeterminantGauss(ValType eps = static_cast<ValType>(1e-10)) const
    requires std::floating_point<ValType>;

  ValType CountDeterminantBareiss() const
    requires std::signed_integral<ValType>;

  BaseMatrix Invert(ValType eps = static_cast<ValType>(1e-10)) const
    requires std::floating_point<ValType>;
  ;
};
} // namespace QuadraticMatrix

#include "CommonDeterminantCountMixin_impl.hpp"
