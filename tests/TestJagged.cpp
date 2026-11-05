#include "JaggedMatrix.hpp"
#include "gtest/gtest.h"

using namespace QuadraticMatrix;

TEST(CreationJaggeds, ZeroMatrixInt) {
  JaggedMatrix<int> matrix(5);
  ASSERT_EQ(5, matrix.GetSize());
}

TEST(CreationJagged, ZeroMatrixDouble) {
  JaggedMatrix<double> matrix(5);
  ASSERT_EQ(5, matrix.GetSize());
}

TEST(CreationJagged, FromIntBuffer) {
  std::vector<int> arr{1, 2, 3, 4};
  JaggedMatrix<int> matrix(2, arr.begin(), arr.end());
  ASSERT_EQ(2, matrix.GetSize());
  ASSERT_EQ(matrix[0][0], 1);
  ASSERT_EQ(matrix[0][1], 2);
  ASSERT_EQ(matrix[1][0], 3);
  ASSERT_EQ(matrix[1][1], 4);
}

TEST(CreationJagged, ConstFromIntBuffer) {
  std::vector<int> arr{1, 2, 3, 4};
  const JaggedMatrix<int> matrix(2, arr.begin(), arr.end());
  ASSERT_EQ(2, matrix.GetSize());
  ASSERT_EQ(matrix[0][0], 1);
  ASSERT_EQ(matrix[0][1], 2);
  ASSERT_EQ(matrix[1][0], 3);
  ASSERT_EQ(matrix[1][1], 4);
}

TEST(CreationJagged, FromDoubleBuffer) {
  std::vector<double> arr{1.5, 2.5, 3.5, 4.5};
  JaggedMatrix<double> matrix(2, arr.begin(), arr.end());
  ASSERT_EQ(2, matrix.GetSize());
  ASSERT_DOUBLE_EQ(matrix[0][0], 1.5);
  ASSERT_DOUBLE_EQ(matrix[0][1], 2.5);
  ASSERT_DOUBLE_EQ(matrix[1][0], 3.5);
  ASSERT_DOUBLE_EQ(matrix[1][1], 4.5);
}

TEST(CreationJagged, ConstFromDoubleBuffer) {
  std::vector<double> arr{1.5, 2.5, 3.5, 4.5};
  const JaggedMatrix<double> matrix(2, arr.begin(), arr.end());
  ASSERT_EQ(2, matrix.GetSize());
  ASSERT_DOUBLE_EQ(matrix[0][0], 1.5);
  ASSERT_DOUBLE_EQ(matrix[0][1], 2.5);
  ASSERT_DOUBLE_EQ(matrix[1][0], 3.5);
  ASSERT_DOUBLE_EQ(matrix[1][1], 4.5);
}

TEST(CreationJagged, From2DIntBuffer) {
  std::vector<std::vector<int>> arr{{1, 2}, {3, 4}};
  JaggedMatrix<int> matrix(arr);
  ASSERT_EQ(2, matrix.GetSize());
  ASSERT_EQ(matrix[0][0], 1);
  ASSERT_EQ(matrix[0][1], 2);
  ASSERT_EQ(matrix[1][0], 3);
  ASSERT_EQ(matrix[1][1], 4);
}

TEST(CreationJagged, ConstFrom2DIntBuffer) {
  std::vector<std::vector<int>> arr{{1, 2}, {3, 4}};
  const JaggedMatrix<int> matrix(arr);
  ASSERT_EQ(2, matrix.GetSize());
  ASSERT_EQ(matrix[0][0], 1);
  ASSERT_EQ(matrix[0][1], 2);
  ASSERT_EQ(matrix[1][0], 3);
  ASSERT_EQ(matrix[1][1], 4);
}

TEST(CreationJagged, From2DDoubleBuffer) {
  std::vector<std::vector<double>> arr{{1.5, 2.5}, {3.5, 4.5}};
  JaggedMatrix<double> matrix(arr);
  ASSERT_EQ(2, matrix.GetSize());
  ASSERT_DOUBLE_EQ(matrix[0][0], 1.5);
  ASSERT_DOUBLE_EQ(matrix[0][1], 2.5);
  ASSERT_DOUBLE_EQ(matrix[1][0], 3.5);
  ASSERT_DOUBLE_EQ(matrix[1][1], 4.5);
}

TEST(CreationJagged, ConstFrom2DDoubleBuffer) {
  std::vector<std::vector<double>> arr{{1.5, 2.5}, {3.5, 4.5}};
  const JaggedMatrix<double> matrix(arr);
  ASSERT_EQ(2, matrix.GetSize());
  ASSERT_DOUBLE_EQ(matrix[0][0], 1.5);
  ASSERT_DOUBLE_EQ(matrix[0][1], 2.5);
  ASSERT_DOUBLE_EQ(matrix[1][0], 3.5);
  ASSERT_DOUBLE_EQ(matrix[1][1], 4.5);
}

TEST(EqualityJagged, DoubleMatrixEQ) {
  std::vector<std::vector<double>> arr{{1.5, 2.5}, {3.5, 4.5}};
  JaggedMatrix<double> matrix(arr);
  JaggedMatrix<double> matrix_second(arr);
  ASSERT_EQ(matrix.Equal(matrix_second), true);
}

TEST(EqualityJagged, DoubleMatrixNotEQ) {
  std::vector<std::vector<double>> arr{{1.5, 2.5}, {3.5, 4.5}};
  std::vector<std::vector<double>> arr1{{2.5, 2.5}, {3.5, 4.5}};
  JaggedMatrix<double> matrix(arr);
  JaggedMatrix<double> matrix_second(arr1);
  ASSERT_EQ(matrix.Equal(matrix_second), false);
}

TEST(EqualityJagged, IntMatrixEQ) {
  std::vector<std::vector<int>> arr{{1, 2}, {3, 4}};
  JaggedMatrix<int> matrix(arr);
  JaggedMatrix<int> matrix_second(arr);
  ASSERT_EQ(matrix.Equal(matrix_second), true);
}

TEST(EqualityJagged, IntMatrixNotEQ) {
  std::vector<std::vector<int>> arr{{1, 2}, {3, 4}};
  std::vector<std::vector<int>> arr1{{0, 2}, {3, 4}};
  JaggedMatrix<int> matrix(arr);
  JaggedMatrix<int> matrix_second(arr1);
  ASSERT_EQ(matrix.Equal(matrix_second), false);
}

TEST(RAIIJagged, MoveInt) {
  std::vector<std::vector<int>> arr{{1, 2}, {3, 4}};
  std::vector<std::vector<int>> arr1{{1, 2}, {3, 4}};
  JaggedMatrix<int> matrix(arr);
  JaggedMatrix<int> matrix_one(std::move(arr1));
  ASSERT_EQ(matrix.Equal(matrix_one), true);
}

TEST(RAIIJagged, MoveEquality) {
  std::vector<std::vector<int>> arr{{1, 2}, {3, 4}};
  JaggedMatrix<int> matrix(arr);
  JaggedMatrix<int> matrix_one = std::move(matrix);
  ASSERT_EQ(matrix.Equal(matrix_one), true);
}

TEST(RAIIJagged, ReqEquality) {
  std::vector<std::vector<int>> arr{{1, 2}, {3, 4}};
  JaggedMatrix<int> matrix(arr);
  JaggedMatrix<int> matrix_one = matrix;
  ASSERT_EQ(matrix.Equal(matrix_one), true);
}

TEST(EyeJagged, Eye) {
  std::vector<std::vector<int>> arr{{1, 0}, {0, 1}};
  JaggedMatrix<int> matrix(arr);
  auto eye = JaggedMatrix<int>::Eye(2);
  ASSERT_EQ(eye.Equal(matrix), true);
}

TEST(TransposeJagged, Transpose) {
  std::vector<std::vector<int>> arr{{1, 3}, {4, 2}};
  std::vector<std::vector<int>> arr1{{1, 4}, {3, 2}};
  JaggedMatrix<int> matrix(arr);
  matrix = matrix.Transpose();
  JaggedMatrix<int> matrix_one(arr1);

  ASSERT_EQ(matrix_one.Equal(matrix), true);
}

TEST(InversionJagged, BadMatrixSimple) {
  JaggedMatrix<double> matrix(5);
  EXPECT_ANY_THROW(matrix.Invert());
}

TEST(InversionJagged, BadMatrixHarder) {
  JaggedMatrix<double> matrix = JaggedMatrix<double>::Eye(5);
  matrix[4][4] = 0;
  EXPECT_ANY_THROW(matrix.Invert());
}

TEST(InversionJagged, BaseUsecase) {
  std::vector<std::vector<double>> arr{{2, 1}, {5, 3}};
  std::vector<std::vector<double>> arr1{{3, -1}, {-5, 2}};
  JaggedMatrix<double> matrix_one(arr);
  JaggedMatrix<double> matrix_two(arr1);
  auto matrixxyz = matrix_one.Invert();
  ASSERT_NEAR(matrix_two[0][0], matrixxyz[0][0], 1e-9);
  ASSERT_NEAR(matrix_two[1][0], matrixxyz[1][0], 1e-9);
  ASSERT_NEAR(matrix_two[0][1], matrixxyz[0][1], 1e-9);
  ASSERT_NEAR(matrix_two[1][1], matrixxyz[1][1], 1e-9);
}

TEST(DeterminantJagged, Det2D) {
  std::vector<std::vector<int>> arr{{2, 1}, {5, 3}};
  JaggedMatrix<int> matrix(arr);
  ASSERT_EQ(matrix.CountDeterminantBareiss(), 1);
}

TEST(DeterminantJagged, Det2DDouble) {
  std::vector<std::vector<double>> arr{{2, 1}, {5, 3}};
  JaggedMatrix<double> matrix(arr);
  ASSERT_NEAR(matrix.CountDeterminantGauss(), 1.0, 1e-15);
}

TEST(DeterminantJagged, IntDiagonal) {
  std::vector<std::vector<int>> arr{{5, 0, 0}, {0, 3, 0}, {0, 0, 1}};
  JaggedMatrix<int> matrix(arr);
  ASSERT_EQ(matrix.CountDeterminantBareiss(), 15);
}

TEST(DeterminantJagged, DoubleDiagonal) {
  std::vector<std::vector<double>> arr{{5, 0, 0}, {0, 3, 0}, {0, 0, 0.2}};
  JaggedMatrix<double> matrix(arr);
  ASSERT_EQ(matrix.CountDeterminantGauss(), 3);
}

TEST(DeterminantJagged, ConstIntDiagonal) {
  std::vector<std::vector<int>> arr{{5, 0, 0}, {0, 3, 0}, {0, 0, 1}};
  const JaggedMatrix<int> matrix(arr);
  ASSERT_EQ(matrix.CountDeterminantBareiss(), 15);
}

TEST(DeterminantJagged, ConstDoubleDiagonal) {
  std::vector<std::vector<double>> arr{{5, 0, 0}, {0, 3, 0}, {0, 0, 0.2}};
  const JaggedMatrix<double> matrix(arr);
  ASSERT_EQ(matrix.CountDeterminantGauss(), 3);
}

TEST(DeterminantJagged, IntTriangular) {
  std::vector<std::vector<int>> arr{{5, 0, 0}, {1, 3, 0}, {7, 8, 1}};
  JaggedMatrix<int> matrix(arr);
  ASSERT_EQ(matrix.CountDeterminantBareiss(), 15);
}

TEST(DeterminantJagged, DoubleTriangular) {
  std::vector<std::vector<double>> arr{
      {5, 0, 0}, {67, 3, 0}, {0.00001, 52.52, 0.2}};
  JaggedMatrix<double> matrix(arr);
  ASSERT_NEAR(matrix.CountDeterminantGauss(), 3, 1e-10);
}

TEST(MultiplicationJagged, DoubleDefaultCase) {
  std::vector<std::vector<double>> arr_one{{1, 2}, {3, 4}};
  std::vector<std::vector<double>> arr_two{{0.25, 0.5}, {0.5, 0.25}};
  std::vector<std::vector<double>> res_arr{{1.25, 1.0}, {2.75, 2.5}};
  JaggedMatrix<double> matrix_one(arr_one);
  JaggedMatrix<double> matrix_two(arr_two);
  JaggedMatrix<double> res_matrix(res_arr);
  auto res = matrix_one * matrix_two;
  ASSERT_EQ(res_matrix.Equal(res), true);
}

TEST(MultiplicationJagged, DoubleErrorRightCase) {
  std::vector<std::vector<double>> arr_one{{1, 2}, {3, 4}};
  std::vector<std::vector<double>> arr_two{{0.25}};
  std::vector<std::vector<double>> res_arr{{1.25, 1.0}, {2.75, 2.5}};
  JaggedMatrix<double> matrix_one(arr_one);
  JaggedMatrix<double> matrix_two(arr_two);
  JaggedMatrix<double> res_matrix(res_arr);
  EXPECT_ANY_THROW(matrix_one * matrix_two);
}

TEST(MultiplicationJagged, DoubleErrorLeftCase) {
  std::vector<std::vector<double>> arr_one{{1}};
  std::vector<std::vector<double>> arr_two{{0.25, 0.5}, {0.5, 0.25}};
  std::vector<std::vector<double>> res_arr{{1.25, 1.0}, {2.75, 2.5}};
  JaggedMatrix<double> matrix_one(arr_one);
  JaggedMatrix<double> matrix_two(arr_two);
  JaggedMatrix<double> res_matrix(res_arr);
  EXPECT_ANY_THROW(matrix_one * matrix_two);
}

TEST(MultiplicationJagged, IntDefaultCase) {
  std::vector<std::vector<int>> arr_one{{1, 2}, {3, 4}};
  std::vector<std::vector<int>> arr_two{{2, 3}, {4, 5}};
  std::vector<std::vector<int>> res_arr{{10, 13}, {22, 29}};
  JaggedMatrix<int> matrix_one(arr_one);
  JaggedMatrix<int> matrix_two(arr_two);
  JaggedMatrix<int> res_matrix(res_arr);
  auto res = matrix_one * matrix_two;
  ASSERT_EQ(res_matrix.Equal(res), true);
}

TEST(MultiplicationJagged, IntErrorRightCase) {
  std::vector<std::vector<int>> arr_one{{1, 2}, {3, 4}};
  std::vector<std::vector<int>> arr_two{{1}};
  std::vector<std::vector<int>> res_arr{{10, 13}, {23, 29}};
  JaggedMatrix<int> matrix_one(arr_one);
  JaggedMatrix<int> matrix_two(arr_two);
  JaggedMatrix<int> res_matrix(res_arr);
  EXPECT_ANY_THROW(matrix_one * matrix_two);
}

TEST(MultiplicationJagged, IntErrorLeftCase) {
  std::vector<std::vector<int>> arr_one{{1, 2}, {3, 4}};
  std::vector<std::vector<int>> arr_two{{1}};
  std::vector<std::vector<int>> res_arr{{10, 13}, {23, 29}};
  JaggedMatrix<int> matrix_one(arr_one);
  JaggedMatrix<int> matrix_two(arr_two);
  JaggedMatrix<int> res_matrix(res_arr);
  EXPECT_ANY_THROW(matrix_one * matrix_two);
}