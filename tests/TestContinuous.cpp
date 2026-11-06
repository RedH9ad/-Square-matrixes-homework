#include "ContinuousMatrix.hpp"
#include "gtest/gtest.h"
#include <cmath>
#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace QuadraticMatrix;

static std::string RunE2EPipeline(std::istream &in) {
  size_t n = 0;
  if (!(in >> n)) {
    return "";
  }
  std::vector<int> buffer;
  int val = 0;
  for (size_t i = 0; i < n * n; ++i) {
    in >> val;
    buffer.push_back(val);
  }
  ContinuousMatrix<int> mat(n, buffer.begin(), buffer.end());
  std::ostringstream out;
  out << mat.CountDeterminantBareiss();
  return out.str();
}

TEST(ContinuousCreation, SizeZero) {
  ContinuousMatrix<int> m0(0);
  EXPECT_EQ(m0.GetSize(), 0);
  EXPECT_EQ(m0.CountDeterminantBareiss(), 0);

  ContinuousMatrix<double> md0(0);
  EXPECT_EQ(md0.GetSize(), 0);
  EXPECT_DOUBLE_EQ(md0.CountDeterminantGauss(), 0.0);
}

TEST(ContinuousCreation, SizeOne) {
  std::vector<int> buf{42};
  ContinuousMatrix<int> m(1, buf.begin(), buf.end());
  EXPECT_EQ(m.GetSize(), 1);
  EXPECT_EQ(m[0][0], 42);
  EXPECT_EQ(m.CountDeterminantBareiss(), 42);
}

TEST(ContinuousCreation, SizeFiveDefaultZero) {
  ContinuousMatrix<int> m(5);
  EXPECT_EQ(m.GetSize(), 5);
  for (size_t i = 0; i < 5; ++i) {
    for (size_t j = 0; j < 5; ++j) {
      EXPECT_EQ(m[i][j], 0);
    }
  }
}

TEST(ContinuousCreation, SizeTenDefaultZero) {
  ContinuousMatrix<double> m(10);
  EXPECT_EQ(m.GetSize(), 10);
  for (size_t i = 0; i < 10; ++i) {
    for (size_t j = 0; j < 10; ++j) {
      EXPECT_DOUBLE_EQ(m[i][j], 0.0);
    }
  }
}

TEST(ContinuousCreation, InvalidBufferSizes) {
  std::vector<int> short_buf{1, 2, 3};
  EXPECT_THROW(ContinuousMatrix<int>(2, short_buf.begin(), short_buf.end()),
               std::invalid_argument);

  std::vector<int> long_buf{1, 2, 3, 4, 5};
  EXPECT_THROW(ContinuousMatrix<int>(2, long_buf.begin(), long_buf.end()),
               std::invalid_argument);

  std::vector<double> buf_3x3(8, 1.0);
  EXPECT_THROW(ContinuousMatrix<double>(3, buf_3x3.begin(), buf_3x3.end()),
               std::invalid_argument);
}

TEST(ContinuousCreation, NonSquareVectorThrows) {
  std::vector<std::vector<int>> non_square_2x3{{1, 2, 3}, {4, 5, 6}};
  EXPECT_THROW(ContinuousMatrix<int> m(non_square_2x3), std::runtime_error);

  std::vector<std::vector<double>> non_square_4x2{
      {1.0, 2.0}, {3.0, 4.0}, {5.0, 6.0}, {7.0, 8.0}};
  EXPECT_THROW(ContinuousMatrix<double> m(non_square_4x2), std::runtime_error);
}

TEST(ContinuousAccess, NegativeValues) {
  ContinuousMatrix<int> m({{-10, -20, -30}, {-40, -50, -60}, {-70, -80, -90}});
  EXPECT_EQ(m[0][0], -10);
  EXPECT_EQ(m[1][1], -50);
  EXPECT_EQ(m[2][2], -90);

  m[1][1] = 500;
  EXPECT_EQ(m[1][1], 500);
}

TEST(ContinuousAccess, LargeIntValues) {
  ContinuousMatrix<int> m({{1000000, -2000000}, {1500000, 214748364}});
  EXPECT_EQ(m[0][0], 1000000);
  EXPECT_EQ(m[0][1], -2000000);
  EXPECT_EQ(m[1][0], 1500000);
  EXPECT_EQ(m[1][1], 214748364);
}

TEST(ContinuousAccess, TinyDoubleValues) {
  ContinuousMatrix<double> m({{1e-9, -2.5e-11}, {3.14e-15, 8.8e-8}});
  EXPECT_DOUBLE_EQ(m[0][0], 1e-9);
  EXPECT_DOUBLE_EQ(m[0][1], -2.5e-11);
  EXPECT_DOUBLE_EQ(m[1][0], 3.14e-15);
  EXPECT_DOUBLE_EQ(m[1][1], 8.8e-8);
}

TEST(ContinuousAccess, LargeDoubleValues) {
  ContinuousMatrix<double> m({{1e12, -2.5e14}, {3.14e10, 8.8e15}});
  EXPECT_DOUBLE_EQ(m[0][0], 1e12);
  EXPECT_DOUBLE_EQ(m[0][1], -2.5e14);
  EXPECT_DOUBLE_EQ(m[1][0], 3.14e10);
  EXPECT_DOUBLE_EQ(m[1][1], 8.8e15);
}

TEST(ContinuousRAII, MoveConstructorSizeTwo) {
  std::vector<std::vector<int>> data{{1, 2}, {3, 4}};
  ContinuousMatrix<int> source(data);
  ContinuousMatrix<int> target(std::move(source));

  EXPECT_EQ(source.GetSize(), 0);
  ASSERT_EQ(target.GetSize(), 2);
  EXPECT_EQ(target[0][0], 1);
  EXPECT_EQ(target[0][1], 2);
  EXPECT_EQ(target[1][0], 3);
  EXPECT_EQ(target[1][1], 4);
}

TEST(ContinuousRAII, MoveConstructorSizeFive) {
  ContinuousMatrix<double> source = ContinuousMatrix<double>::Eye(5);
  ContinuousMatrix<double> target(std::move(source));

  EXPECT_EQ(source.GetSize(), 0);
  ASSERT_EQ(target.GetSize(), 5);
  for (size_t i = 0; i < 5; ++i) {
    for (size_t j = 0; j < 5; ++j) {
      if (i == j) {
        EXPECT_DOUBLE_EQ(target[i][j], 1.0);
      } else {
        EXPECT_DOUBLE_EQ(target[i][j], 0.0);
      }
    }
  }
}

TEST(ContinuousRAII, MoveAssignmentSizeThree) {
  std::vector<std::vector<int>> data(
      {{10, 20, 30}, {40, 50, 60}, {70, 80, 90}});
  ContinuousMatrix<int> source(data);
  ContinuousMatrix<int> target(1);

  target = std::move(source);

  EXPECT_EQ(source.GetSize(), 1);
  ASSERT_EQ(target.GetSize(), 3);
  EXPECT_EQ(target[0][0], 10);
  EXPECT_EQ(target[2][2], 90);
}

TEST(ContinuousRAII, MoveSelfAssignment) {
  ContinuousMatrix<int> m({{7, 8, 9}, {1, 2, 3}, {4, 5, 6}});

  m = std::move(m);

  EXPECT_EQ(m.GetSize(), 3);
  EXPECT_EQ(m[0][0], 7);
  EXPECT_EQ(m[2][2], 6);
}

TEST(ContinuousRAII, CopyIndependenceSizeFour) {
  ContinuousMatrix<double> original = ContinuousMatrix<double>::Eye(4);
  ContinuousMatrix<double> copy_constructed = original;

  copy_constructed[0][0] = 999.0;
  EXPECT_DOUBLE_EQ(original[0][0], 1.0);
  EXPECT_DOUBLE_EQ(copy_constructed[0][0], 999.0);

  ContinuousMatrix<double> copy_assigned(4);
  copy_assigned = original;
  copy_assigned[1][1] = 777.0;
  EXPECT_DOUBLE_EQ(original[1][1], 1.0);
  EXPECT_DOUBLE_EQ(copy_assigned[1][1], 777.0);
}

TEST(ContinuousEquality, IntSizesAndValues) {
  ContinuousMatrix<int> m1({{-1000, 2000}, {3000, -4000}});
  ContinuousMatrix<int> m2({{-1000, 2000}, {3000, -4000}});
  ContinuousMatrix<int> m3({{-1000, 2000}, {3000, -4001}});

  EXPECT_TRUE(m1.Equal(m2));
  EXPECT_FALSE(m1.Equal(m3));
}

TEST(ContinuousEquality, DoubleEpsilonTolerance) {
  ContinuousMatrix<double> m1({{1.0, 2.0000000001}, {3.0, 4.0}});
  ContinuousMatrix<double> m2({{1.0, 2.0000000002}, {3.0, 4.0}});

  EXPECT_TRUE(m1.Equal(m2, 1e-9));
  EXPECT_FALSE(m1.Equal(m2, 1e-11));
}

TEST(ContinuousEquality, DimensionMismatch) {
  ContinuousMatrix<int> m2(2);
  ContinuousMatrix<int> m3(3);
  EXPECT_FALSE(m2.Equal(m3));
}

TEST(ContinuousArithmetic, AdditionSmallInt) {
  ContinuousMatrix<int> a({{1, 2}, {3, 4}});
  ContinuousMatrix<int> b({{5, 6}, {7, 8}});
  ContinuousMatrix<int> expected({{6, 8}, {10, 12}});

  auto res = a + b;
  EXPECT_TRUE(res.Equal(expected));
}

TEST(ContinuousArithmetic, AdditionLargeInt) {
  ContinuousMatrix<int> a({{1000000, -2000000}, {3000000, 4000000}});
  ContinuousMatrix<int> b({{5000000, 1000000}, {-2000000, 6000000}});
  ContinuousMatrix<int> expected({{6000000, -1000000}, {1000000, 10000000}});

  auto res = a + b;
  EXPECT_TRUE(res.Equal(expected));
}

TEST(ContinuousArithmetic, AdditionNegativeValues) {
  ContinuousMatrix<int> a({{-5, -10, -15}, {-20, -25, -30}, {-35, -40, -45}});
  ContinuousMatrix<int> b({{5, 10, 15}, {20, 25, 30}, {35, 40, 45}});
  ContinuousMatrix<int> expected(3);

  auto res = a + b;
  EXPECT_TRUE(res.Equal(expected));
}

TEST(ContinuousArithmetic, AdditionDoubleDifferentScales) {
  ContinuousMatrix<double> a({{1e-5, 1e5}, {-2e-5, 3e5}});
  ContinuousMatrix<double> b({{4e-5, 2e5}, {2e-5, -1e5}});
  ContinuousMatrix<double> expected({{5e-5, 3e5}, {0.0, 2e5}});

  auto res = a + b;
  EXPECT_TRUE(res.Equal(expected, 1e-9));
}

TEST(ContinuousArithmetic, AdditionDimensionMismatch) {
  ContinuousMatrix<int> a(2);
  ContinuousMatrix<int> b(3);
  EXPECT_THROW(auto res = a + b, std::runtime_error);
}

TEST(ContinuousArithmetic, MultiplicationSizeOne) {
  ContinuousMatrix<int> a(1);
  ContinuousMatrix<int> b(1);
  a[0][0] = -7;
  b[0][0] = 6;
  auto res = a * b;
  EXPECT_EQ(res.GetSize(), 1);
  EXPECT_EQ(res[0][0], -42);
}

TEST(ContinuousArithmetic, MultiplicationNegativeInts) {
  ContinuousMatrix<int> a({{-1, -2}, {-3, -4}});
  ContinuousMatrix<int> b({{2, -3}, {-4, 5}});
  ContinuousMatrix<int> expected({{6, -7}, {10, -11}});

  auto res = a * b;
  EXPECT_TRUE(res.Equal(expected));
}

TEST(ContinuousArithmetic, MultiplicationIdentityPropertySizeFour) {
  ContinuousMatrix<double> a({{1.5, -2.0, 3.1, 4.2},
                              {0.5, 6.2, -1.1, 0.0},
                              {-9.0, 1.2, 3.4, -5.6},
                              {7.1, 8.2, 0.0, 1.1}});
  auto eye = ContinuousMatrix<double>::Eye(4);

  auto res1 = a * eye;
  auto res2 = eye * a;

  EXPECT_TRUE(res1.Equal(a, 1e-12));
  EXPECT_TRUE(res2.Equal(a, 1e-12));
}

TEST(ContinuousArithmetic, MultiplicationNilpotentSizeFour) {
  ContinuousMatrix<int> n(
      {{0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}, {0, 0, 0, 0}});
  ContinuousMatrix<int> zero(4);

  auto n2 = n * n;
  auto n4 = n2 * n2;

  EXPECT_FALSE(n2.Equal(zero));
  EXPECT_TRUE(n4.Equal(zero));
}

TEST(ContinuousArithmetic, MultiplicationLargeValues) {
  ContinuousMatrix<int64_t> a({{100000, 200000}, {300000, 400000}});
  ContinuousMatrix<int64_t> b({{2, 0}, {0, 2}});
  ContinuousMatrix<int64_t> expected({{200000, 400000}, {600000, 800000}});

  auto res = a * b;
  EXPECT_TRUE(res.Equal(expected));
}

TEST(ContinuousArithmetic, TransposeSizeOne) {
  ContinuousMatrix<int> m(1);
  m[0][0] = 99;
  EXPECT_TRUE(m.Transpose().Equal(m));
}

TEST(ContinuousArithmetic, TransposeSizeThree) {
  ContinuousMatrix<int> m({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
  ContinuousMatrix<int> expected({{1, 4, 7}, {2, 5, 8}, {3, 6, 9}});

  EXPECT_TRUE(m.Transpose().Equal(expected));
  EXPECT_TRUE(m.Transpose().Transpose().Equal(m));
}

TEST(ContinuousArithmetic, TransposeSizeFive) {
  ContinuousMatrix<double> m({{1.0, 2.0, 3.0, 4.0, 5.0},
                              {6.0, 7.0, 8.0, 9.0, 10.0},
                              {11.0, 12.0, 13.0, 14.0, 15.0},
                              {16.0, 17.0, 18.0, 19.0, 20.0},
                              {21.0, 22.0, 23.0, 24.0, 25.0}});

  auto t = m.Transpose();
  for (size_t i = 0; i < 5; ++i) {
    for (size_t j = 0; j < 5; ++j) {
      EXPECT_DOUBLE_EQ(t[i][j], m[j][i]);
    }
  }
}

TEST(ContinuousDeterminantBareiss, SizeZero) {
  ContinuousMatrix<int> m(0);
  EXPECT_EQ(m.CountDeterminantBareiss(), 0);
}

TEST(ContinuousDeterminantBareiss, SizeOne) {
  ContinuousMatrix<int> m(1);
  m[0][0] = -123;
  EXPECT_EQ(m.CountDeterminantBareiss(), -123);
}

TEST(ContinuousDeterminantBareiss, SizeTwoBasic) {
  ContinuousMatrix<int> m({{1, 5}, {3, 2}});
  EXPECT_EQ(m.CountDeterminantBareiss(), -13);
}

TEST(ContinuousDeterminantBareiss, SizeTwoOverflow) {
  ContinuousMatrix<int> m({{50000, 49999}, {49999, 49998}});
  EXPECT_EQ(m.CountDeterminantBareiss(), -1);
}

TEST(ContinuousDeterminantBareiss, SizeTwoExtremeOverflow) {
  ContinuousMatrix<int> m({{70000, 69999}, {69999, 69998}});
  EXPECT_EQ(m.CountDeterminantBareiss(), -1);
}

TEST(ContinuousDeterminantBareiss, SizeThreePermutationRequired) {
  ContinuousMatrix<int> m({{0, 1, 2}, {1, 0, 3}, {4, -1, 0}});
  EXPECT_EQ(m.CountDeterminantBareiss(), 10);
}

TEST(ContinuousDeterminantBareiss, SizeThreeSingular) {
  ContinuousMatrix<int> m({{1, 2, 3}, {2, 4, 6}, {7, 8, 9}});
  EXPECT_EQ(m.CountDeterminantBareiss(), 0);
}

TEST(ContinuousDeterminantBareiss, SizeFourTriangular) {
  ContinuousMatrix<int> m(
      {{2, 3, 1, 5}, {0, -3, 2, 4}, {0, 0, 4, -1}, {0, 0, 0, -2}});
  EXPECT_EQ(m.CountDeterminantBareiss(), 48);
}

TEST(ContinuousDeterminantBareiss, SizeFiveDiagonalWithNegatives) {
  ContinuousMatrix<int> m({{-2, 0, 0, 0, 0},
                           {0, -3, 0, 0, 0},
                           {0, 0, -1, 0, 0},
                           {0, 0, 0, 5, 0},
                           {0, 0, 0, 0, 2}});
  EXPECT_EQ(m.CountDeterminantBareiss(), -60);
}

TEST(ContinuousDeterminantBareiss, SizeSixAntiDiagonal) {
  ContinuousMatrix<int> m({{0, 0, 0, 0, 0, 1},
                           {0, 0, 0, 0, 1, 0},
                           {0, 0, 0, 1, 0, 0},
                           {0, 0, 1, 0, 0, 0},
                           {0, 1, 0, 0, 0, 0},
                           {1, 0, 0, 0, 0, 0}});
  EXPECT_EQ(m.CountDeterminantBareiss(), -1);
}

TEST(ContinuousDeterminantBareiss, MultiplicativePropertySizeThree) {
  ContinuousMatrix<int> a({{2, 1, 0}, {1, 2, 1}, {0, 1, 2}});
  ContinuousMatrix<int> b({{3, 0, 1}, {2, 2, 0}, {-1, 1, 4}});

  auto c = a * b;
  EXPECT_EQ(c.CountDeterminantBareiss(),
            a.CountDeterminantBareiss() * b.CountDeterminantBareiss());
}

TEST(ContinuousDeterminantGauss, SizeZero) {
  ContinuousMatrix<double> m(0);
  EXPECT_DOUBLE_EQ(m.CountDeterminantGauss(), 0.0);
}

TEST(ContinuousDeterminantGauss, SizeOne) {
  ContinuousMatrix<double> m(1);
  m[0][0] = -3.1415926535;
  EXPECT_NEAR(m.CountDeterminantGauss(), -3.1415926535, 1e-9);
}

TEST(ContinuousDeterminantGauss, SizeTwoTinyDoubles) {
  ContinuousMatrix<double> m({{1e-5, 0.0}, {0.0, 2e-5}});
  EXPECT_NEAR(m.CountDeterminantGauss(), 2e-10, 1e-15);
}

TEST(ContinuousDeterminantGauss, SizeThreeLargeDoubles) {
  ContinuousMatrix<double> m(
      {{1e6, 0.0, 0.0}, {5e5, 2e6, 0.0}, {1e4, 2e4, 3e6}});
  EXPECT_NEAR(m.CountDeterminantGauss(), 6e18, 1e10);
}

TEST(ContinuousDeterminantGauss, SizeFourAntiDiagonal) {
  ContinuousMatrix<double> m({{0.0, 0.0, 0.0, 2.0},
                              {0.0, 0.0, 3.0, 0.0},
                              {0.0, 4.0, 0.0, 0.0},
                              {5.0, 0.0, 0.0, 0.0}});
  EXPECT_NEAR(m.CountDeterminantGauss(), 120.0, 1e-9);
}

TEST(ContinuousDeterminantGauss, SizeFiveUpperTriangular) {
  ContinuousMatrix<double> m({{2.0, 1.5, -4.2, 0.8, 9.1},
                              {0.0, -3.0, 7.7, 1.2, 0.0},
                              {0.0, 0.0, 1.0, 5.5, -6.1},
                              {0.0, 0.0, 0.0, 4.0, 3.3},
                              {0.0, 0.0, 0.0, 0.0, -1.0}});
  EXPECT_NEAR(m.CountDeterminantGauss(), 24.0, 1e-9);
}

TEST(ContinuousDeterminantGauss, SizeThreeSingular) {
  ContinuousMatrix<double> m(
      {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}});
  EXPECT_NEAR(m.CountDeterminantGauss(), 0.0, 1e-9);
}

TEST(ContinuousInversion, SizeOne) {
  ContinuousMatrix<double> m(1);
  m[0][0] = 4.0;
  auto inv = m.Invert();
  EXPECT_EQ(inv.GetSize(), 1);
  EXPECT_DOUBLE_EQ(inv[0][0], 0.25);
}

TEST(ContinuousInversion, SizeTwoTinyValues) {
  ContinuousMatrix<double> m({{1e-4, 0.0}, {0.0, 2e-4}});
  auto inv = m.Invert();
  EXPECT_NEAR(inv[0][0], 10000.0, 1e-5);
  EXPECT_NEAR(inv[1][1], 5000.0, 1e-5);
}

TEST(ContinuousInversion, SizeTwoLargeValues) {
  ContinuousMatrix<double> m({{10000.0, 0.0}, {0.0, 20000.0}});
  auto inv = m.Invert();
  EXPECT_NEAR(inv[0][0], 0.0001, 1e-9);
  EXPECT_NEAR(inv[1][1], 0.00005, 1e-9);
}

TEST(ContinuousInversion, SizeThreeGeneral) {
  ContinuousMatrix<double> a(
      {{1.0, 2.0, 3.0}, {0.0, 1.0, 4.0}, {5.0, 6.0, 0.0}});
  auto inv = a.Invert();
  auto identity = a * inv;
  auto eye = ContinuousMatrix<double>::Eye(3);

  EXPECT_TRUE(identity.Equal(eye, 1e-7));
}

TEST(ContinuousInversion, SizeFourGeneral) {
  ContinuousMatrix<double> a({{4.0, 0.0, 0.0, 1.0},
                              {0.0, 3.0, 2.0, 0.0},
                              {1.0, 2.0, 3.0, 0.0},
                              {0.0, 0.0, 1.0, 2.0}});
  auto inv = a.Invert();
  auto identity = a * inv;
  auto eye = ContinuousMatrix<double>::Eye(4);

  EXPECT_TRUE(identity.Equal(eye, 1e-7));
}

TEST(ContinuousInversion, SizeFiveIdentity) {
  auto eye = ContinuousMatrix<double>::Eye(5);
  auto inv = eye.Invert();
  EXPECT_TRUE(inv.Equal(eye, 1e-12));
}

TEST(ContinuousInversion, SingularSizeOneThrows) {
  ContinuousMatrix<double> m(1);
  m[0][0] = 0.0;
  EXPECT_THROW(m.Invert(), std::runtime_error);
}

TEST(ContinuousInversion, SingularSizeThreeThrows) {
  ContinuousMatrix<double> m(
      {{1.0, 2.0, 3.0}, {2.0, 4.0, 6.0}, {1.0, 1.0, 1.0}});
  EXPECT_THROW(m.Invert(), std::runtime_error);
}

TEST(ContinuousE2E, SizeOne) {
  std::istringstream input_stream("1 -17");
  std::string output = RunE2EPipeline(input_stream);
  EXPECT_EQ(output, "-17");
}

TEST(ContinuousE2E, SizeTwoSlideExample) {
  std::istringstream input_stream("2 1 0 0 1");
  std::string output = RunE2EPipeline(input_stream);
  EXPECT_EQ(output, "1");
}

TEST(ContinuousE2E, SizeTwoOverflow) {
  std::istringstream input_stream("2 50000 49999 49999 49998");
  std::string output = RunE2EPipeline(input_stream);
  EXPECT_EQ(output, "-1");
}

TEST(ContinuousE2E, SizeThreeGeneral) {
  std::istringstream input_stream("3 1 2 3 0 1 4 5 6 0");
  std::string output = RunE2EPipeline(input_stream);
  EXPECT_EQ(output, "1");
}

TEST(ContinuousE2E, SizeFourTriangular) {
  std::istringstream input_stream("4  2 3 1 5  0 -3 2 4  0 0 4 -1  0 0 0 -2");
  std::string output = RunE2EPipeline(input_stream);
  EXPECT_EQ(output, "48");
}