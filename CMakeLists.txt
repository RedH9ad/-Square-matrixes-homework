cmake_minimum_required(VERSION 4.4.2)
project(MatrixDeterminant)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)

FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
    googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG        v1.8.3
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googlebenchmark)

enable_testing()

add_library(matrix_core INTERFACE)
target_include_directories(matrix_core INTERFACE matrix_src)
add_library(generator_interface INTERFACE)
target_include_directories(generator_interface INTERFACE test_generator)

add_executable(matrix_tests
    tests/TestContinuous.cpp
    tests/TestJagged.cpp
)

add_executable(benchmark_det benchmark/benchmark_determinant.cpp)
add_executable(benchmark_mul benchmark/benchmark_multiplication.cpp)
add_executable(benchmark_add benchmark/benchmark_addition.cpp)

target_link_libraries(benchmark_det
    PRIVATE
        matrix_core
        generator_interface
        benchmark::benchmark_main
)

target_link_libraries(benchmark_mul
    PRIVATE
        matrix_core
        generator_interface
        benchmark::benchmark_main
)

target_link_libraries(benchmark_add
    PRIVATE
        matrix_core
        generator_interface
        benchmark::benchmark_main
)

target_link_libraries(matrix_tests
    PRIVATE
        GTest::gtest_main
        matrix_core
)

include(GoogleTest)
gtest_discover_tests(matrix_tests)

option(ENABLE_SANITIZERS "Enable ASan and UBSan" OFF)

if(ENABLE_SANITIZERS)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=address,undefined)
endif()