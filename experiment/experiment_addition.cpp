#include "ContinuousMatrix.hpp"
#include "JaggedMatrix.hpp"
#include "TestGenerator.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

struct ExperimentRecord {
  double cont_time;
  double jagg_time;
  size_t matrix_size;
};

void WriteToCSV(const std::string &filename,
                const std::vector<ExperimentRecord> &records,
                std::mutex &mutex);

template <class ValType>
void MakeExperimentInt(const std::string &filename, size_t iterations,
                       ValType vlb, ValType vub, size_t slb, size_t sub,
                       std::mutex &mutex, unsigned int seed = 42) {
  auto generator = MatrixDatasetsGenerator<ValType>(vlb, vub, slb, sub, seed);

  std::vector<ExperimentRecord> buffer;
  buffer.reserve(iterations);

  for (size_t step = 0; step < iterations; step++) {
    auto temp = generator.GenerateBuffer();
    auto mult = generator.GenerateBuffer(temp.size());
    QuadraticMatrix::ContinuousMatrix<ValType> cont_matrix(temp);
    QuadraticMatrix::JaggedMatrix<ValType> jagg_matrix(temp);
    QuadraticMatrix::ContinuousMatrix<ValType> cont_matrix_mult(mult);
    QuadraticMatrix::JaggedMatrix<ValType> jagg_matrix_mult(mult);

    auto start_cont = std::chrono::high_resolution_clock::now();
    auto garb_cont = cont_matrix + cont_matrix_mult;
    auto end_cont = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> cont_time = end_cont - start_cont;

    auto start_jagg = std::chrono::high_resolution_clock::now();
    auto garb_jagg = jagg_matrix + jagg_matrix_mult;
    auto end_jagg = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> jagg_time = end_jagg - start_jagg;

    buffer.push_back(
        {cont_time.count(), jagg_time.count(), cont_matrix.GetSize()});
  }

  WriteToCSV(filename, buffer, mutex);
}

template <class ValType>
void MakeExperimentFloating(const std::string &filename, size_t iterations,
                            ValType vlb, ValType vub, size_t slb, size_t sub,
                            std::mutex &mutex, unsigned int seed = 42) {
  auto generator = MatrixDatasetsGenerator<ValType>(vlb, vub, slb, sub, seed);

  std::vector<ExperimentRecord> buffer;
  buffer.reserve(iterations);

  for (size_t step = 0; step < iterations; step++) {
    auto temp = generator.GenerateBuffer();
    auto mult = generator.GenerateBuffer(temp.size());
    QuadraticMatrix::ContinuousMatrix<ValType> cont_matrix(temp);
    QuadraticMatrix::JaggedMatrix<ValType> jagg_matrix(temp);
    QuadraticMatrix::ContinuousMatrix<ValType> cont_matrix_mult(mult);
    QuadraticMatrix::JaggedMatrix<ValType> jagg_matrix_mult(mult);

    auto start_cont = std::chrono::high_resolution_clock::now();
    auto garb_cont = cont_matrix + cont_matrix_mult;
    auto end_cont = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> cont_time = end_cont - start_cont;

    auto start_jagg = std::chrono::high_resolution_clock::now();
    auto garb_jagg = jagg_matrix + jagg_matrix_mult;
    auto end_jagg = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> jagg_time = end_jagg - start_jagg;

    buffer.push_back(
        {cont_time.count(), jagg_time.count(), cont_matrix.GetSize()});
  }

  WriteToCSV(filename, buffer, mutex);
}

void WriteToCSV(const std::string &filename,
                const std::vector<ExperimentRecord> &records,
                std::mutex &mutex) {
  std::scoped_lock locker(mutex);

  std::ifstream check_file(filename);
  bool is_empty = check_file.peek() == std::ifstream::traits_type::eof();
  check_file.close();

  std::ofstream file(filename, std::ios::app);

  if (is_empty) {
    file << "ContinuousTime_ms,JaggedTime_ms,MatrixSize\n";
  }

  for (const auto &entry : records) {
    file << entry.cont_time << "," << entry.jagg_time << ","
         << entry.matrix_size << "\n";
  }
}

int main() {
  size_t size_lb = 2;
  size_t size_ub = 1500;
  size_t iterations = 67;
  int vlb = -100;
  int vub = 100;
  std::mutex mutex;
  size_t thread_size = std::thread::hardware_concurrency() - 1;
  std::cout << "Эксперимент начинается" << "\n";

  std::string int_filename("../experiment_data/int_add.csv");
  std::vector<std::thread> threads;
  for (size_t i = 0; i < thread_size; i++) {
    threads.emplace_back(&MakeExperimentInt<int>, std::cref(int_filename),
                         iterations, vlb, vub, size_lb, size_ub,
                         std::ref(mutex), 42 + i);
  }
  for (auto &thread : threads) {
    thread.join();
  }
  std::cout << "Обработка интов завершена" << '\n';

  std::string long_filename("../experiment_data/long_add.csv");
  std::vector<std::thread> threads_long;
  for (size_t i = 0; i < thread_size; i++) {
    threads_long.emplace_back(&MakeExperimentInt<long>,
                              std::cref(long_filename), iterations,
                              static_cast<long>(vlb), static_cast<long>(vub),
                              size_lb, size_ub, std::ref(mutex), 42 + i);
  }
  for (auto &thread : threads_long) {
    thread.join();
  }
  std::cout << "Обработка лонгов завершена" << '\n';

  std::string longlong_filename("../experiment_data/longlong_add.csv");
  std::vector<std::thread> threads_longlong;
  for (size_t i = 0; i < thread_size; i++) {
    threads_longlong.emplace_back(
        &MakeExperimentInt<long long>, std::cref(longlong_filename), iterations,
        static_cast<long long>(vlb), static_cast<long long>(vub), size_lb,
        size_ub, std::ref(mutex), 42 + i);
  }
  for (auto &thread : threads_longlong) {
    thread.join();
  }
  std::cout << "Обработка лонг лонгов завершена" << '\n';

  std::string double_filename("../experiment_data/double_add.csv");
  std::vector<std::thread> threads_double;
  for (size_t i = 0; i < thread_size; i++) {
    threads_double.emplace_back(
        &MakeExperimentFloating<double>, std::cref(double_filename), iterations,
        static_cast<double>(vlb), static_cast<double>(vub), size_lb, size_ub,
        std::ref(mutex), 42 + i);
  }
  for (auto &thread : threads_double) {
    thread.join();
  }
  std::cout << "Обработка даблов завершена" << '\n';

  std::string float_filename("../experiment_data/float_add.csv");
  std::vector<std::thread> threads_float;
  for (size_t i = 0; i < thread_size; i++) {
    threads_float.emplace_back(&MakeExperimentFloating<float>,
                               std::cref(float_filename), iterations,
                               static_cast<float>(vlb), static_cast<float>(vub),
                               size_lb, size_ub, std::ref(mutex), 42 + i);
  }
  for (auto &thread : threads_float) {
    thread.join();
  }
  std::cout << "Обработка флотов завершена" << '\n';
}