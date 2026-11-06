#include <benchmark/benchmark.h>

#include "ContinuousMatrix.hpp"
#include "JaggedMatrix.hpp"
#include "TestGenerator.hpp"

#include <type_traits>
#include <vector>

using namespace QuadraticMatrix;

static constexpr int value_lower_bound = -100;
static constexpr int value_upper_bound = 100;

template <typename T>
std::vector<std::vector<T>> CreateMatrixBuffer(size_t size,
                                               unsigned int seed = 42) {
  if constexpr (std::is_integral_v<T>) {
    MatrixDatasetsGenerator<T> generator(static_cast<T>(value_lower_bound),
                                         static_cast<T>(value_upper_bound),
                                         size, size, seed);
    return generator.GenerateBuffer(size);
  } else {
    MatrixDatasetsGenerator<T> generator(static_cast<T>(value_lower_bound),
                                         static_cast<T>(value_upper_bound),
                                         size, size, seed);
    return generator.GenerateBuffer(size);
  }
}

template <template <class> class Matrix, typename ValType>
void BenchmarkMultiplication(benchmark::State &state) {
  size_t size = state.range(0);
  Matrix<ValType> first(CreateMatrixBuffer<ValType>(size, 88));
  Matrix<ValType> second(CreateMatrixBuffer<ValType>(size, 89));

  for (auto _ : state) {
    auto res = first * second;
    benchmark::DoNotOptimize(res);
    benchmark::ClobberMemory();
  }
  state.SetComplexityN(size);
}

BENCHMARK_TEMPLATE(BenchmarkMultiplication, ContinuousMatrix, int)
    ->DenseRange(800, 1500, 30)
    ->Complexity(benchmark::oNSquared);
BENCHMARK_TEMPLATE(BenchmarkMultiplication, JaggedMatrix, int)
    ->DenseRange(800, 1500, 30)
    ->Complexity(benchmark::oNSquared);
BENCHMARK_TEMPLATE(BenchmarkMultiplication, ContinuousMatrix, double)
    ->DenseRange(800, 1500, 30)
    ->Complexity(benchmark::oNSquared);
BENCHMARK_TEMPLATE(BenchmarkMultiplication, JaggedMatrix, double)
    ->DenseRange(800, 1500, 30)
    ->Complexity(benchmark::oNSquared);

BENCHMARK_MAIN();