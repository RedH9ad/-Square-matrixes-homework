#pragma once
#include "CommonDeterminantCountMixin.hpp"
#include <cstddef>
#include <stdexcept>

namespace QuadraticMatrix {
template <class BaseMatrix, class ValType>
void DeterminantMixin<BaseMatrix, ValType>::AddMultipliedRows(
    size_t id_from, size_t id_to, ValType multiplier) {
  for (size_t i = 0; i < matrix()->GetSize(); i++) {
    (*matrix())[id_to][i] += multiplier * (*matrix())[id_from][i];
  }
}

template <class BaseMatrix, class ValType>
size_t DeterminantMixin<BaseMatrix, ValType>::FindMaxPivot(bool &zero_column,
                                                           size_t iteration,
                                                           ValType eps) {
  size_t max_pivot = iteration;
  auto max_val = std::abs((*matrix())[iteration][iteration]);
  for (size_t pivot_finder = iteration + 1; pivot_finder < matrix()->GetSize();
       pivot_finder++) {
    if (std::abs((*matrix())[pivot_finder][iteration]) > max_val) {
      max_pivot = pivot_finder;
      max_val = std::abs((*matrix())[pivot_finder][iteration]);
    }
  }
  if (max_val < eps) {
    zero_column = true;
  }
  return max_pivot;
}

template <class BaseMatrix, class ValType>
void DeterminantMixin<BaseMatrix, ValType>::ProcessConsistenIterationGauss(
    size_t iteration) {
  for (size_t row = iteration + 1; row < matrix()->GetSize(); row++) {
    ValType multiplier =
        (*matrix())[row][iteration] / (*matrix())[iteration][iteration];
    AddMultipliedRows(iteration, row, -multiplier);
  }
}

template <class BaseMatrix, class ValType>
void DeterminantMixin<BaseMatrix, ValType>::ToTriangular(bool &string_change,
                                                         bool &zero_column,
                                                         ValType eps) {
  string_change = false;
  zero_column = false;
  for (size_t iteration = 0; iteration < matrix()->GetSize(); iteration++) {
    size_t max_pivot = FindMaxPivot(zero_column, iteration, eps);
    if (max_pivot != iteration) {
      matrix()->SwapTwoRows(iteration, max_pivot);
      string_change = !string_change;
    }
    if (zero_column) {
      return;
    }
    ProcessConsistenIterationGauss(iteration);
  }
}

template <class BaseMatrix, class ValType>
ValType
DeterminantMixin<BaseMatrix, ValType>::CountDeterminantGauss(ValType eps) const
  requires std::floating_point<ValType>
{
  if (matrix()->GetSize() == 0) {
    return static_cast<ValType>(0);
  }
  BaseMatrix temp = *matrix();
  bool string_change = false;
  bool zero_column = false;
  temp.ToTriangular(string_change, zero_column, eps);
  if (zero_column) {
    return static_cast<ValType>(0);
  }
  ValType det = static_cast<ValType>(1);
  for (size_t row = 0; row < matrix()->GetSize(); row++) {
    det *= temp[row][row];
  }

  if (string_change) {
    det *= static_cast<ValType>(-1);
  }
  return det;
}

template <class BaseMatrix, class ValType>
void DeterminantMixin<BaseMatrix, ValType>::ProcessZeroMinor(
    size_t iteration, bool &row_change, bool &zero_column) {
  for (size_t pivot_finder = iteration + 1; pivot_finder < matrix()->GetSize();
       pivot_finder++) {
    if ((*matrix())[pivot_finder][iteration] != static_cast<ValType>(0)) {
      matrix()->SwapTwoRows(iteration, pivot_finder);
      row_change = !row_change;
      return;
    }
  }
  zero_column = true;
}

template <class BaseMatrix, class ValType>
void DeterminantMixin<BaseMatrix, ValType>::ProcessConsistenIterationBareiss(
    size_t iteration, ValType previous_minor) {
  for (size_t row = iteration + 1; row < matrix()->GetSize(); row++) {
    for (size_t col = iteration + 1; col < matrix()->GetSize(); col++) {
      (*matrix())[row][col] =
          ((*matrix())[row][col] * (*matrix())[iteration][iteration] -
           (*matrix())[row][iteration] * (*matrix())[iteration][col]) /
          previous_minor;
    }
  }
}

template <class BaseMatrix, class ValType>
ValType DeterminantMixin<BaseMatrix, ValType>::CountDeterminantBareiss() const
  requires std::signed_integral<ValType>
{
  if (matrix()->GetSize() == 0) {
    return static_cast<ValType>(0);
  }

  BaseMatrix res = *matrix();
  bool row_change = false;
  bool zero_column = false;
  ValType previous_minor = static_cast<ValType>(1);
  for (size_t iteration = 0; iteration < matrix()->GetSize() - 1; iteration++) {
    if (iteration != 0) {
      previous_minor = res[iteration - 1][iteration - 1];
    }

    if (res[iteration][iteration] == static_cast<ValType>(0)) {
      res.ProcessZeroMinor(iteration, row_change, zero_column);
      if (zero_column) {
        return static_cast<ValType>(0);
      }
    }
    res.ProcessConsistenIterationBareiss(iteration, previous_minor);
  }
  ValType det = res[matrix()->GetSize() - 1][matrix()->GetSize() - 1];
  if (row_change) {
    det *= -1;
  }
  return det;
}

template <class BaseMatrix, class ValType>
void DeterminantMixin<BaseMatrix, ValType>::OneInvertionIteration(
    size_t iteration, ValType eps, BaseMatrix &res) {

  bool zero_column = false;
  size_t max_pivot = FindMaxPivot(zero_column, iteration, eps);
  if (zero_column) {
    throw std::runtime_error("Попытка обратить вырожденную матрицу");
  }
  if (max_pivot != iteration) {
    matrix()->SwapTwoRows(iteration, max_pivot);
    res.SwapTwoRows(iteration, max_pivot);
  }
  for (size_t row = 0; row < matrix()->GetSize(); row++) {
    if (row == iteration) {
      continue;
    }
    ValType multiplier =
        (*matrix())[row][iteration] / (*matrix())[iteration][iteration];
    AddMultipliedRows(iteration, row, -multiplier);
    res.AddMultipliedRows(iteration, row, -multiplier);
  }

  ValType multiplier = (*matrix())[iteration][iteration];
  for (size_t col = 0; col < matrix()->GetSize(); col++) {
    (*matrix())[iteration][col] /= multiplier;
    res[iteration][col] /= multiplier;
  }
}

template <class BaseMatrix, class ValType>
BaseMatrix DeterminantMixin<BaseMatrix, ValType>::Invert(ValType eps) const {
  BaseMatrix res = matrix()->Eye(matrix()->GetSize());
  BaseMatrix temp(*matrix());
  for (size_t iteration = 0; iteration < matrix()->GetSize(); iteration++) {
    temp.OneInvertionIteration(iteration, eps, res);
  }
  return res;
}
} // namespace QuadraticMatrix