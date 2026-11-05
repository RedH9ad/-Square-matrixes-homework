#pragma once
#include "TestGenerator.hpp"
#include <random>
#include <vector>

template <class ValType>
MatrixDatasetsGenerator<ValType>::MatrixDatasetsGenerator(
    ValType vlb, ValType vub, size_t slb, size_t sub, unsigned int seed)
    : val_lower_bound_(vlb), val_upper_bound_(vub), size_lower_bound_(slb),
      size_upper_bound_(sub), rng_(seed){};

template <class ValType>
std::vector<std::vector<ValType>>
MatrixDatasetsGenerator<ValType>::GenerateBuffer() {
  std::uniform_int_distribution<size_t> size_dist(size_lower_bound_,
                                                  size_upper_bound_);
  size_t matrix_size = size_dist(rng_);
  std::vector<std::vector<ValType>> matrix(matrix_size,
                                           std::vector<ValType>(matrix_size));

  if constexpr (std::is_integral_v<ValType>) {
    std::uniform_int_distribution<ValType> dist(val_lower_bound_,
                                                val_upper_bound_);
    for (size_t row = 0; row < matrix_size; row++) {
      for (size_t col = 0; col < matrix_size; col++) {
        matrix[row][col] = dist(rng_);
      }
    }
  }

  if constexpr (std::is_floating_point_v<ValType>) {
    std::uniform_real_distribution<ValType> dist(val_lower_bound_,
                                                 val_upper_bound_);
    for (size_t row = 0; row < matrix_size; row++) {
      for (size_t col = 0; col < matrix_size; col++) {
        matrix[row][col] = dist(rng_);
      }
    }
  }
  return matrix;
}

template <class ValType>
std::vector<std::vector<ValType>>
MatrixDatasetsGenerator<ValType>::GenerateBuffer(size_t size) {
  size_t matrix_size = size;
  std::vector<std::vector<ValType>> matrix(matrix_size,
                                           std::vector<ValType>(matrix_size));

  if constexpr (std::is_integral_v<ValType>) {
    std::uniform_int_distribution<ValType> dist(val_lower_bound_,
                                                val_upper_bound_);
    for (size_t row = 0; row < matrix_size; row++) {
      for (size_t col = 0; col < matrix_size; col++) {
        matrix[row][col] = dist(rng_);
      }
    }
  }

  if constexpr (std::is_floating_point_v<ValType>) {
    std::uniform_real_distribution<ValType> dist(val_lower_bound_,
                                                 val_upper_bound_);
    for (size_t row = 0; row < matrix_size; row++) {
      for (size_t col = 0; col < matrix_size; col++) {
        matrix[row][col] = dist(rng_);
      }
    }
  }
  return matrix;
}