#include "pch.h"

#include "LinAlg/MatrixStencil.hpp"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;
using namespace testing;


TEST(AMGStandardCoarseningTest, TestStrongConnectionsForRowForFivePointStencil) {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil << 0, -1,  0,
              -1,  4, -1,
               0, -1,  0;

    SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);

//    m.print();
/*
      4       -1        0       -1        0        0        0        0        0
     -1        4       -1        0       -1        0        0        0        0
      0       -1        4        0        0       -1        0        0        0
     -1        0        0        4       -1        0       -1        0        0
      0       -1        0       -1        4       -1        0       -1        0
      0        0       -1        0       -1        4        0        0       -1
      0        0        0       -1        0        0        4       -1        0
      0        0        0        0       -1        0       -1        4       -1
      0        0        0        0        0       -1        0       -1        4
*/

    AMGStandardStrengthPolicy strength_policy(m);

    // row 0
    auto variable_set = strength_policy.GetInfluencedByVariables(0);
    ASSERT_TRUE(variable_set->contains(1)) << "Variable 0 should not have a strong connection to variable 1";
    ASSERT_TRUE(variable_set->contains(3)) << "Variable 0 should not have a strong connection to variable 3";
    ASSERT_TRUE(variable_set->size() == 2) << "Variable 0 should not have a strong connection on itself";

    // row 3
    variable_set = strength_policy.GetInfluencedByVariables(3);
    ASSERT_TRUE(variable_set->contains(0)) << "Variable 3 should not have a strong connection to variable 0";
    ASSERT_TRUE(variable_set->contains(4)) << "Variable 3 should not have a strong connection to variable 4";
    ASSERT_TRUE(variable_set->contains(6)) << "Variable 3 should not have a strong connection to variable 6";
    ASSERT_TRUE(variable_set->size() == 3) << "Variable 3 should not have a strong connection on itself";
}

TEST(AMGStandardCoarseningTest, TestStrongConnectionsForRowForNinePointStencil) {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil << -1, -4,  -1,
               -4, 20,  -4,
               -1, -4,  -1;

    SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);

//    m.print();
/*
      20       -4        0       -4       -1        0        0        0        0
      -4       20       -4       -1       -4       -1        0        0        0
       0       -4       20        0       -1       -4        0        0        0
      -4       -1        0       20       -4        0       -4       -1        0
      -1       -4       -1       -4       20       -4       -1       -4       -1
       0       -1       -4        0       -4       20        0       -1       -4
       0        0        0       -4       -1        0       20       -4        0
       0        0        0       -1       -4       -1       -4       20       -4
       0        0        0        0       -1       -4        0       -4       20
*/

    AMGStandardStrengthPolicy strength_policy(m);

    // row 0
    auto variable_set = strength_policy.GetInfluencedByVariables(0);
    ASSERT_TRUE(variable_set->contains(1)) << "Variable 0 should not have a strong connection to variable 1";
    ASSERT_TRUE(variable_set->contains(3)) << "Variable 0 should not have a strong connection to variable 3";
    ASSERT_TRUE(variable_set->contains(4)) << "Variable 0 should not have a strong connection to variable 4";
    ASSERT_TRUE(variable_set->size() == 3) << "Variable 0 should not have a strong connection on itself";

    // row 1
    variable_set = strength_policy.GetInfluencedByVariables(1);
    ASSERT_TRUE(variable_set->contains(0)) << "Variable 1 should not have a strong connection to variable 0";
    ASSERT_TRUE(variable_set->contains(2)) << "Variable 1 should not have a strong connection to variable 2";
    ASSERT_TRUE(variable_set->contains(3)) << "Variable 1 should not have a strong connection to variable 3";
    ASSERT_TRUE(variable_set->contains(4)) << "Variable 1 should not have a strong connection to variable 4";
    ASSERT_TRUE(variable_set->contains(5)) << "Variable 1 should not have a strong connection to variable 5";
    ASSERT_TRUE(variable_set->size() == 5) << "Variable 1 should not have a strong connection on itself";

    // row 4
    variable_set = strength_policy.GetInfluencedByVariables(4);
    ASSERT_TRUE(variable_set->contains(0)) << "Variable 4 should not have a strong connection to variable 0";
    ASSERT_TRUE(variable_set->contains(1)) << "Variable 4 should not have a strong connection to variable 1";
    ASSERT_TRUE(variable_set->contains(2)) << "Variable 4 should not have a strong connection to variable 2";
    ASSERT_TRUE(variable_set->contains(3)) << "Variable 4 should not have a strong connection to variable 3";
    ASSERT_TRUE(variable_set->contains(5)) << "Variable 4 should not have a strong connection to variable 5";
    ASSERT_TRUE(variable_set->contains(6)) << "Variable 4 should not have a strong connection to variable 6";
    ASSERT_TRUE(variable_set->contains(7)) << "Variable 4 should not have a strong connection to variable 7";
    ASSERT_TRUE(variable_set->contains(8)) << "Variable 4 should not have a strong connection to variable 8";
    ASSERT_TRUE(variable_set->size() == 8) << "Variable 4 should not have a strong connection on itself";

    ASSERT_TRUE(LinAlg_NS::helper::isSymmetric(m));
}

TEST(AMGStandardCoarseningTest, TestStrongConnectionsForRowForNinePointStencil2) {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil << -1.0 / 2.0,  -2,          -1.0 / 2.0,
               -1,           29.0 / 4.0, -1,
               -1.0 / 8.0,  -2,          -1.0 / 8.0;

    SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);

//    m.print();
/*
    7.25       -1        0       -2   -0.125        0        0        0        0
      -1     7.25       -1   -0.125       -2   -0.125        0        0        0
       0       -1     7.25        0   -0.125       -2        0        0        0
      -2     -0.5        0     7.25       -1        0       -2   -0.125        0
    -0.5       -2     -0.5       -1     7.25       -1   -0.125       -2   -0.125
       0     -0.5       -2        0       -1     7.25        0   -0.125       -2
       0        0        0       -2     -0.5        0     7.25       -1        0
       0        0        0     -0.5       -2     -0.5       -1     7.25       -1
       0        0        0        0     -0.5       -2        0       -1     7.25
*/

    AMGStandardStrengthPolicy strength_policy(m);

    // row 0: threshold = 2 * eps = 2 * 0.25 = 0.5
    auto variable_set = strength_policy.GetInfluencedByVariables(0);
    ASSERT_TRUE(variable_set->contains(1)) << "Variable 0 should not have a strong connection to variable 1";
    ASSERT_TRUE(variable_set->contains(3)) << "Variable 0 should not have a strong connection to variable 3";
    ASSERT_TRUE(variable_set->size() == 2) << "Variable 0 should not have a strong connection on itself";

    // row 1: threshold = 2 * eps = 2 * 0.25 = 0.5
    variable_set = strength_policy.GetInfluencedByVariables(1);
    ASSERT_TRUE(variable_set->contains(0)) << "Variable 1 should not have a strong connection to variable 0";
    ASSERT_TRUE(variable_set->contains(2)) << "Variable 1 should not have a strong connection to variable 2";
    ASSERT_TRUE(variable_set->contains(4)) << "Variable 1 should not have a strong connection to variable 4";
    ASSERT_TRUE(variable_set->size() == 3) << "Variable 1 should not have a strong connection on itself";

    // row 3: threshold = 2 * eps = 2 * 0.25 = 0.5
    variable_set = strength_policy.GetInfluencedByVariables(3);
    ASSERT_TRUE(variable_set->contains(0)) << "Variable 3 should not have a strong connection to variable 0";
    ASSERT_TRUE(variable_set->contains(1)) << "Variable 3 should not have a strong connection to variable 1";
    ASSERT_TRUE(variable_set->contains(4)) << "Variable 3 should not have a strong connection to variable 4";
    ASSERT_TRUE(variable_set->contains(6)) << "Variable 3 should not have a strong connection to variable 6";
    ASSERT_TRUE(variable_set->size() == 4) << "Variable 1 should not have a strong connection on itself";

    // row 7: threshold = 2 * eps = 2 * 0.25 = 0.5
    variable_set = strength_policy.GetInfluencedByVariables(7);
    ASSERT_TRUE(variable_set->contains(3)) << "Variable 7 should not have a strong connection to variable 3";
    ASSERT_TRUE(variable_set->contains(4)) << "Variable 7 should not have a strong connection to variable 4";
    ASSERT_TRUE(variable_set->contains(5)) << "Variable 7 should not have a strong connection to variable 5";
    ASSERT_TRUE(variable_set->contains(6)) << "Variable 7 should not have a strong connection to variable 6";
    ASSERT_TRUE(variable_set->contains(8)) << "Variable 7 should not have a strong connection to variable 8";
    ASSERT_TRUE(variable_set->size() == 5) << "Variable 7 should not have a strong connection on itself";

    // row 8: threshold = 2 * eps = 2 * 0.25 = 0.5
    variable_set = strength_policy.GetInfluencedByVariables(8);
    ASSERT_TRUE(variable_set->contains(4)) << "Variable 8 should not have a strong connection to variable 4";
    ASSERT_TRUE(variable_set->contains(5)) << "Variable 8 should not have a strong connection to variable 5";
    ASSERT_TRUE(variable_set->contains(7)) << "Variable 8 should not have a strong connection to variable 7";
    ASSERT_TRUE(variable_set->size() == 3) << "Variable 8 should not have a strong connection on itself";

    ASSERT_FALSE(LinAlg_NS::helper::isSymmetric(m));
}

TEST(AMGStandardCoarseningTest, TestStrongConnectionsForStrongFFConnection) {
    SparseMatrix2D m{5};

    // i
    m(0, 0) = 4;
    m(0, 1) = -1;
    m(0, 2) = -1;

    // k
    m(1, 1) = 4;
    m(1, 3) = -1;

    // l
    m(2, 2) = 4;
    m(2, 4) = -1;

    // m
    m(3, 1) = -1;
    m(3, 3) = 4;

    // n
    m(4, 2) = -1;
    m(4, 4) = -1;

    m.finalize();

//    m.print();
/*
            column 0      column 1      column 2      column 3      column 4
        row 0         4.00        -1.00        -1.00            0            0
        row 1            0         4.00            0        -1.00            0
        row 2            0            0         4.00            0        -1.00
        row 3            0        -1.00            0         4.00            0
        row 4            0            0        -1.00            0        -1.00
  */

    AMGStandardStrengthPolicy strength_policy(m);

    // row 0 (i)
    auto variable_set = strength_policy.GetInfluencedByVariables(0);
    ASSERT_TRUE(variable_set->contains(1)) << "Variable 0 should not have a strong connection to variable 1";
    ASSERT_TRUE(variable_set->contains(2)) << "Variable 0 should not have a strong connection to variable 2";
    ASSERT_TRUE(variable_set->size() == 2) << "Variable 0 should not have a strong connection on itself";

    // row 1 (k)
    variable_set = strength_policy.GetInfluencedByVariables(1);
    ASSERT_TRUE(variable_set->contains(3)) << "Variable 1 should not have a strong connection to variable 3";
    ASSERT_TRUE(variable_set->size() == 1) << "Variable 1 should not have a strong connection on itself";

    // row 2 (l)
    variable_set = strength_policy.GetInfluencedByVariables(2);
    ASSERT_TRUE(variable_set->contains(4)) << "Variable 2 should not have a strong connection to variable 4";
    ASSERT_TRUE(variable_set->size() == 1) << "Variable 2 should not have a strong connection on itself";

    // row 3 (m)
    variable_set = strength_policy.GetInfluencedByVariables(3);
    ASSERT_TRUE(variable_set->contains(1)) << "Variable 3 should not have a strong connection to variable 1";
    ASSERT_TRUE(variable_set->size() == 1) << "Variable 3 should not have a strong connection on itself";

    // row 4 (n)
    variable_set = strength_policy.GetInfluencedByVariables(4);
    ASSERT_TRUE(variable_set->contains(2)) << "Variable 4 should not have a strong connection to variable 2";
    ASSERT_TRUE(variable_set->size() == 1) << "Variable 4 should not have a strong connection on itself";
}
