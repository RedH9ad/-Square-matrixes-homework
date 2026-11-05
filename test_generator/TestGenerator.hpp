#pragma once
#include "random"
#include "vector"
#include <type_traits>
#include <vector>

template <class ValType> class MatrixDatasetsGenerator {
  ValType val_lower_bound_;
  ValType val_upper_bound_;
  size_t size_lower_bound_;
  size_t size_upper_bound_;
  std::mt19937 rng_;

public:
  MatrixDatasetsGenerator(ValType vlb, ValType vub, size_t slb, size_t sub,
                          unsigned int seed = std::random_device{}());
  std::vector<std::vector<ValType>> GenerateBuffer();
  std::vector<std::vector<ValType>> GenerateBuffer(size_t size);
};

#include "TestGenerator_impl.hpp"