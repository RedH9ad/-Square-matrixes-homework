#include "ContinuousMatrix.hpp"
#include "gtest/gtest.h"
#include <stdexcept>
#include <utility>
#include <vector>

using namespace QuadraticMatrix;

TEST(CreationContinuouss, ZeroMatrixInt) {
  ContinuousMatrix<int> matrix(5);
  ASSERT_EQ(5, matrix.GetSize());
}

TEST(CreationContinuous, ZeroMatrixDouble) {
  ContinuousMatrix<double> matrix(5);
  ASSERT_EQ(5, matrix.GetSize());
}

TEST(CreationContinuous, FromIntBuffer) {
  std::vector<int> arr{1, 2, 3, 4};
  ContinuousMatrix<int> matrix(2, arr.begin(), arr.end());
  ASSERT_EQ(2, matrix.GetSize());
  ASSERT_EQ(matrix[0][0], 1);
  ASSERT_EQ(matrix[0][1], 2);
  ASSERT_EQ(matrix[1][0], 3);
  ASSERT_EQ(matrix[1][1], 4);
}

TEST(CreationContinuous, ConstFromIntBuffer) {
  std::vector<int> arr{1, 2, 3, 4};
  const ContinuousMatrix<int> matrix(2, arr.begin(), arr.end());
  ASSERT_EQ(2, matrix.GetSize());
  ASSERT_EQ(matrix[0][0], 1);
  ASSERT_EQ(matrix[0][1], 2);
  ASSERT_EQ(matrix[1][0], 3);
  ASSERT_EQ(matrix[1][1], 4);
}

TEST(CreationContinuous, FromDoubleBuffer) {
  std::vector<double> arr{1.5, 2.5, 3.5, 4.5};
  ContinuousMatrix<double> matrix(2, arr.begin(), arr.end());
  ASSERT_EQ(2, matrix.GetSize());
  ASSERT_DOUBLE_EQ(matrix[0][0], 1.5);
  ASSERT_DOUBLE_EQ(matrix[0][1], 2.5);
  ASSERT_DOUBLE_EQ(matrix[1][0], 3.5);
  ASSERT_DOUBLE_EQ(matrix[1][1], 4.5);
}

TEST(CreationContinuous, ConstFromDoubleBuffer) {
  std::vector<double> arr{1.5, 2.5, 3.5, 4.5};
  const ContinuousMatrix<double> matrix(2, arr.begin(), arr.end());
  ASSERT_EQ(2, matrix.GetSize());
  ASSERT_DOUBLE_EQ(matrix[0][0], 1.5);
  ASSERT_DOUBLE_EQ(matrix[0][1], 2.5);
  ASSERT_DOUBLE_EQ(matrix[1][0], 3.5);
  ASSERT_DOUBLE_EQ(matrix[1][1], 4.5);
}

TEST(CreationContinuous, From2DIntBuffer) {
  std::vector<std::vector<int>> arr{{1, 2}, {3, 4}};
  ContinuousMatrix<int> matrix(arr);
  ASSERT_EQ(2, matrix.GetSize());
  ASSERT_EQ(matrix[0][0], 1);
  ASSERT_EQ(matrix[0][1], 2);
  ASSERT_EQ(matrix[1][0], 3);
  ASSERT_EQ(matrix[1][1], 4);
}

TEST(CreationContinuous, ConstFrom2DIntBuffer) {
  std::vector<std::vector<int>> arr{{1, 2}, {3, 4}};
  const ContinuousMatrix<int> matrix(arr);
  ASSERT_EQ(2, matrix.GetSize());
  ASSERT_EQ(matrix[0][0], 1);
  ASSERT_EQ(matrix[0][1], 2);
  ASSERT_EQ(matrix[1][0], 3);
  ASSERT_EQ(matrix[1][1], 4);
}

TEST(CreationContinuous, From2DDoubleBuffer) {
  std::vector<std::vector<double>> arr{{1.5, 2.5}, {3.5, 4.5}};
  ContinuousMatrix<double> matrix(arr);
  ASSERT_EQ(2, matrix.GetSize());
  ASSERT_DOUBLE_EQ(matrix[0][0], 1.5);
  ASSERT_DOUBLE_EQ(matrix[0][1], 2.5);
  ASSERT_DOUBLE_EQ(matrix[1][0], 3.5);
  ASSERT_DOUBLE_EQ(matrix[1][1], 4.5);
}

TEST(CreationContinuous, ConstFrom2DDoubleBuffer) {
  std::vector<std::vector<double>> arr{{1.5, 2.5}, {3.5, 4.5}};
  const ContinuousMatrix<double> matrix(arr);
  ASSERT_EQ(2, matrix.GetSize());
  ASSERT_DOUBLE_EQ(matrix[0][0], 1.5);
  ASSERT_DOUBLE_EQ(matrix[0][1], 2.5);
  ASSERT_DOUBLE_EQ(matrix[1][0], 3.5);
  ASSERT_DOUBLE_EQ(matrix[1][1], 4.5);
}

TEST(EqualityContinuous, DoubleMatrixEQ) {
  std::vector<std::vector<double>> arr{{1.5, 2.5}, {3.5, 4.5}};
  ContinuousMatrix<double> matrix(arr);
  ContinuousMatrix<double> matrix_second(arr);
  ASSERT_EQ(matrix.Equal(matrix_second), true);
}

TEST(EqualityContinuous, DoubleMatrixNotEQ) {
  std::vector<std::vector<double>> arr{{1.5, 2.5}, {3.5, 4.5}};
  std::vector<std::vector<double>> arr1{{2.5, 2.5}, {3.5, 4.5}};
  ContinuousMatrix<double> matrix(arr);
  ContinuousMatrix<double> matrix_second(arr1);
  ASSERT_EQ(matrix.Equal(matrix_second), false);
}

TEST(EqualityContinuous, IntMatrixEQ) {
  std::vector<std::vector<int>> arr{{1, 2}, {3, 4}};
  ContinuousMatrix<int> matrix(arr);
  ContinuousMatrix<int> matrix_second(arr);
  ASSERT_EQ(matrix.Equal(matrix_second), true);
}

TEST(EqualityContinuous, IntMatrixNotEQ) {
  std::vector<std::vector<int>> arr{{1, 2}, {3, 4}};
  std::vector<std::vector<int>> arr1{{0, 2}, {3, 4}};
  ContinuousMatrix<int> matrix(arr);
  ContinuousMatrix<int> matrix_second(arr1);
  ASSERT_EQ(matrix.Equal(matrix_second), false);
}

TEST(RAIIContinuous, MoveInt) {
  std::vector<std::vector<int>> arr{{1, 2}, {3, 4}};
  std::vector<std::vector<int>> arr1{{1, 2}, {3, 4}};
  ContinuousMatrix<int> matrix(arr);
  ContinuousMatrix<int> matrix_one(std::move(arr1));
  ASSERT_EQ(matrix.Equal(matrix_one), true);
}

TEST(RAIIContinuous, MoveEquality) {
  std::vector<std::vector<int>> arr{{1, 2}, {3, 4}};
  ContinuousMatrix<int> matrix(arr);
  ContinuousMatrix<int> matrix_one = std::move(matrix);
  ASSERT_EQ(matrix.Equal(matrix_one), true);
}

TEST(RAIIContinuous, ReqEquality) {
  std::vector<std::vector<int>> arr{{1, 2}, {3, 4}};
  ContinuousMatrix<int> matrix(arr);
  ContinuousMatrix<int> matrix_one = matrix;
  ASSERT_EQ(matrix.Equal(matrix_one), true);
}

TEST(EyeContinuous, Eye) {
  std::vector<std::vector<int>> arr{{1, 0}, {0, 1}};
  ContinuousMatrix<int> matrix(arr);
  auto eye = ContinuousMatrix<int>::Eye(2);
  ASSERT_EQ(eye.Equal(matrix), true);
}

TEST(TransposeContinuous, Transpose) {
  std::vector<std::vector<int>> arr{{1, 3}, {4, 2}};
  std::vector<std::vector<int>> arr1{{1, 4}, {3, 2}};
  ContinuousMatrix<int> matrix(arr);
  matrix = matrix.Transpose();
  ContinuousMatrix<int> matrix_one(arr1);

  ASSERT_EQ(matrix_one.Equal(matrix), true);
}

TEST(InversionContinuous, BadMatrixSimple) {
  ContinuousMatrix<double> matrix(5);
  EXPECT_ANY_THROW(matrix.Invert());
}

TEST(InversionContinuous, BadMatrixHarder) {
  ContinuousMatrix<double> matrix = ContinuousMatrix<double>::Eye(5);
  matrix[4][4] = 0;
  EXPECT_ANY_THROW(matrix.Invert());
}

TEST(InversionContinuous, BaseUsecase) {
  std::vector<std::vector<double>> arr{{2, 1}, {5, 3}};
  std::vector<std::vector<double>> arr1{{3, -1}, {-5, 2}};
  ContinuousMatrix<double> matrix_one(arr);
  ContinuousMatrix<double> matrix_two(arr1);
  auto matrixxyz = matrix_one.Invert();
  ASSERT_NEAR(matrix_two[0][0], matrixxyz[0][0], 1e-9);
  ASSERT_NEAR(matrix_two[1][0], matrixxyz[1][0], 1e-9);
  ASSERT_NEAR(matrix_two[0][1], matrixxyz[0][1], 1e-9);
  ASSERT_NEAR(matrix_two[1][1], matrixxyz[1][1], 1e-9);
}

TEST(DeterminantContinuous, Det2D) {
  std::vector<std::vector<int>> arr{{2, 1}, {5, 3}};
  ContinuousMatrix<int> matrix(arr);
  ASSERT_EQ(matrix.CountDeterminantBareiss(), 1);
}

TEST(DeterminantContinuous, Det2DDouble) {
  std::vector<std::vector<double>> arr{{2, 1}, {5, 3}};
  ContinuousMatrix<double> matrix(arr);
  ASSERT_NEAR(matrix.CountDeterminantGauss(), 1.0, 1e-15);
}

TEST(DeterminantContinuous, IntDiagonal) {
  std::vector<std::vector<int>> arr{{5, 0, 0}, {0, 3, 0}, {0, 0, 1}};
  ContinuousMatrix<int> matrix(arr);
  ASSERT_EQ(matrix.CountDeterminantBareiss(), 15);
}

TEST(DeterminantContinuous, DoubleDiagonal) {
  std::vector<std::vector<double>> arr{{5, 0, 0}, {0, 3, 0}, {0, 0, 0.2}};
  ContinuousMatrix<double> matrix(arr);
  ASSERT_EQ(matrix.CountDeterminantGauss(), 3);
}

TEST(DeterminantContinuous, ConstIntDiagonal) {
  std::vector<std::vector<int>> arr{{5, 0, 0}, {0, 3, 0}, {0, 0, 1}};
  const ContinuousMatrix<int> matrix(arr);
  ASSERT_EQ(matrix.CountDeterminantBareiss(), 15);
}

TEST(DeterminantContinuous, ConstDoubleDiagonal) {
  std::vector<std::vector<double>> arr{{5, 0, 0}, {0, 3, 0}, {0, 0, 0.2}};
  const ContinuousMatrix<double> matrix(arr);
  ASSERT_EQ(matrix.CountDeterminantGauss(), 3);
}

TEST(DeterminantContinuous, IntTriangular) {
  std::vector<std::vector<int>> arr{{5, 0, 0}, {1, 3, 0}, {7, 8, 1}};
  ContinuousMatrix<int> matrix(arr);
  ASSERT_EQ(matrix.CountDeterminantBareiss(), 15);
}

TEST(DeterminantContinuous, DoubleTriangular) {
  std::vector<std::vector<double>> arr{
      {5, 0, 0}, {67, 3, 0}, {0.00001, 52.52, 0.2}};
  ContinuousMatrix<double> matrix(arr);
  ASSERT_NEAR(matrix.CountDeterminantGauss(), 3, 1e-10);
}

TEST(MultiplicationContinuous, DoubleDefaultCase) {
  std::vector<std::vector<double>> arr_one{{1, 2}, {3, 4}};
  std::vector<std::vector<double>> arr_two{{0.25, 0.5}, {0.5, 0.25}};
  std::vector<std::vector<double>> res_arr{{1.25, 1.0}, {2.75, 2.5}};
  ContinuousMatrix<double> matrix_one(arr_one);
  ContinuousMatrix<double> matrix_two(arr_two);
  ContinuousMatrix<double> res_matrix(res_arr);
  auto res = matrix_one * matrix_two;
  ASSERT_EQ(res_matrix.Equal(res), true);
}

TEST(MultiplicationContinuous, DoubleErrorRightCase) {
  std::vector<std::vector<double>> arr_one{{1, 2}, {3, 4}};
  std::vector<std::vector<double>> arr_two{{0.25}};
  std::vector<std::vector<double>> res_arr{{1.25, 1.0}, {2.75, 2.5}};
  ContinuousMatrix<double> matrix_one(arr_one);
  ContinuousMatrix<double> matrix_two(arr_two);
  ContinuousMatrix<double> res_matrix(res_arr);
  EXPECT_ANY_THROW(matrix_one * matrix_two);
}

TEST(MultiplicationContinuous, DoubleErrorLeftCase) {
  std::vector<std::vector<double>> arr_one{{1}};
  std::vector<std::vector<double>> arr_two{{0.25, 0.5}, {0.5, 0.25}};
  std::vector<std::vector<double>> res_arr{{1.25, 1.0}, {2.75, 2.5}};
  ContinuousMatrix<double> matrix_one(arr_one);
  ContinuousMatrix<double> matrix_two(arr_two);
  ContinuousMatrix<double> res_matrix(res_arr);
  EXPECT_ANY_THROW(matrix_one * matrix_two);
}

TEST(MultiplicationContinuous, IntDefaultCase) {
  std::vector<std::vector<int>> arr_one{{1, 2}, {3, 4}};
  std::vector<std::vector<int>> arr_two{{2, 3}, {4, 5}};
  std::vector<std::vector<int>> res_arr{{10, 13}, {22, 29}};
  ContinuousMatrix<int> matrix_one(arr_one);
  ContinuousMatrix<int> matrix_two(arr_two);
  ContinuousMatrix<int> res_matrix(res_arr);
  auto res = matrix_one * matrix_two;
  ASSERT_EQ(res_matrix.Equal(res), true);
}

TEST(MultiplicationContinuous, IntErrorRightCase) {
  std::vector<std::vector<int>> arr_one{{1, 2}, {3, 4}};
  std::vector<std::vector<int>> arr_two{{1}};
  std::vector<std::vector<int>> res_arr{{10, 13}, {23, 29}};
  ContinuousMatrix<int> matrix_one(arr_one);
  ContinuousMatrix<int> matrix_two(arr_two);
  ContinuousMatrix<int> res_matrix(res_arr);
  EXPECT_ANY_THROW(matrix_one * matrix_two);
}

TEST(MultiplicationContinuous, IntErrorLeftCase) {
  std::vector<std::vector<int>> arr_one{{1, 2}, {3, 4}};
  std::vector<std::vector<int>> arr_two{{1}};
  std::vector<std::vector<int>> res_arr{{10, 13}, {23, 29}};
  ContinuousMatrix<int> matrix_one(arr_one);
  ContinuousMatrix<int> matrix_two(arr_two);
  ContinuousMatrix<int> res_matrix(res_arr);
  EXPECT_ANY_THROW(matrix_one * matrix_two);
}