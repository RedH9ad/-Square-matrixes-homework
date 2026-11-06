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

template <template <class> class Matrix>
void BenchmarkDeterminantBareiss_Int(benchmark::State &state) {
  size_t size = state.range(0);
  Matrix<int> first(CreateMatrixBuffer<int>(size, 53));

  for (auto _ : state) {
    auto det = first.CountDeterminantBareiss();
    benchmark::DoNotOptimize(det);
    benchmark::ClobberMemory();
  }
  state.SetComplexityN(size);
}

template <template <class> class Matrix>
void BenchmarkDeterminantGauss_Double(benchmark::State &state) {
  size_t size = state.range(0);
  Matrix<double> first(CreateMatrixBuffer<double>(size, 54));

  for (auto _ : state) {
    auto det = first.CountDeterminantGauss();
    benchmark::DoNotOptimize(det);
    benchmark::ClobberMemory();
  }
  state.SetComplexityN(size);
}

BENCHMARK_TEMPLATE(BenchmarkDeterminantBareiss_Int, ContinuousMatrix)
    ->DenseRange(10, 2000, 25)
    ->Complexity(benchmark::oNCubed);
BENCHMARK_TEMPLATE(BenchmarkDeterminantGauss_Double, ContinuousMatrix)
    ->DenseRange(10, 2000, 25)
    ->Complexity(benchmark::oNCubed);

BENCHMARK_TEMPLATE(BenchmarkDeterminantBareiss_Int, JaggedMatrix)
    ->DenseRange(10, 2000, 25)
    ->Complexity(benchmark::oNCubed);
BENCHMARK_TEMPLATE(BenchmarkDeterminantGauss_Double, JaggedMatrix)
    ->DenseRange(10, 2000, 25)
    ->Complexity(benchmark::oNCubed);

BENCHMARK_MAIN();