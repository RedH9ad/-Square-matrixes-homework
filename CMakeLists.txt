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

enable_testing()

add_library(matrix_core INTERFACE)
target_include_directories(matrix_core INTERFACE matrix_src)
add_library(generator_interface INTERFACE)
target_include_directories(generator_interface INTERFACE test_generator)

add_executable(matrix_tests
    tests/TestContinuous.cpp
    tests/TestJagged.cpp
)

add_executable(experiment_det experiment/experiment_determinant.cpp)
add_executable(experiment_mul experiment/experiment_multiplication.cpp)
add_executable(experiment_add experiment/experiment_addition.cpp)

target_link_libraries(experiment_det
    PRIVATE
        matrix_core
        generator_interface
)

target_link_libraries(experiment_mul
    PRIVATE
        matrix_core
        generator_interface
)

target_link_libraries(experiment_add
    PRIVATE
        matrix_core
        generator_interface
)

target_link_libraries(matrix_tests
    PRIVATE
        GTest::gtest_main
        matrix_core
)

include(GoogleTest)
gtest_discover_tests(matrix_tests)
