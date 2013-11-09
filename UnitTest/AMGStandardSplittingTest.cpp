#include "pch.h"

#include "AMGStandardSplittingTest.h"

#include "LinearSolverLibrary/VariableCategorizer.h"
#include "LinearSolverLibrary/VariableCardinalityPolicy.h"
#include "LinearSolverLibrary/VariableInfluenceAccessor.h"
#include "LinearSolverLibrary/AMGStandardCoarseningStrengthPolicy.h"
#include "LinearSolverLibrary/AMGStandardSplitting.h"
#include "LinAlg/SparseMatrix2D.h"
#include "LinAlg/MatrixStencil.h"

using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


void
AMGStandardSplittingTest::setUp() {}

void
AMGStandardSplittingTest::tearDown() {}

void
AMGStandardSplittingTest::TestSplittingForSimple5ptStencil() {
    // from Trottenberg, p. 475, Fig. A.12 top
    MatrixStencil stencil;
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

    AMGStandardCoarseningStrengthPolicy strength_policy(m);
    VariableCategorizer variable_categorizer(m.rows());
    VariableInfluenceAccessor influence_accessor(strength_policy, variable_categorizer);
    AMGStandardSplitting splitting(m, influence_accessor, variable_categorizer);
    splitting.generateSplitting();
    variable_categorizer.print();

/*
    C F C
    F C F
    C F C
*/

    for (auto variable : {0, 2, 4, 6, 8}) {
        auto variable_type = static_cast<char>(variable_categorizer.GetType(variable));
        auto expected = static_cast<char>(VariableCategorizer::Type::COARSE);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong variable type", expected, variable_type);
    }

    for (auto variable : {1, 3, 5, 7}) {
        auto variable_type = static_cast<char>(variable_categorizer.GetType(variable));
        auto expected = static_cast<char>(VariableCategorizer::Type::FINE);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong variable type", expected, variable_type);
    }
}

void
AMGStandardSplittingTest::TestSplittingForSimple9ptStencil() {
    // from Trottenberg, p. 475, Fig. A.12 bottom
    MatrixStencil stencil;
    stencil << -1, -1, -1,
               -1,  8, -1,
               -1, -1, -1;

    SparseMatrix2D const & m = stencil.generateMatrix(5 * 5);

//    m.print();
/*
       8       -1        0        0        0       -1       -1        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0
      -1        8       -1        0        0       -1       -1       -1        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0
       0       -1        8       -1        0        0       -1       -1       -1        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0
       0        0       -1        8       -1        0        0       -1       -1       -1        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0
       0        0        0       -1        8        0        0        0       -1       -1        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0
      -1       -1        0        0        0        8       -1        0        0        0       -1       -1        0        0        0        0        0        0        0        0        0        0        0        0        0
      -1       -1       -1        0        0       -1        8       -1        0        0       -1       -1       -1        0        0        0        0        0        0        0        0        0        0        0        0
       0       -1       -1       -1        0        0       -1        8       -1        0        0       -1       -1       -1        0        0        0        0        0        0        0        0        0        0        0
       0        0       -1       -1       -1        0        0       -1        8       -1        0        0       -1       -1       -1        0        0        0        0        0        0        0        0        0        0
       0        0        0       -1       -1        0        0        0       -1        8        0        0        0       -1       -1        0        0        0        0        0        0        0        0        0        0
       0        0        0        0        0       -1       -1        0        0        0        8       -1        0        0        0       -1       -1        0        0        0        0        0        0        0        0
       0        0        0        0        0       -1       -1       -1        0        0       -1        8       -1        0        0       -1       -1       -1        0        0        0        0        0        0        0
       0        0        0        0        0        0       -1       -1       -1        0        0       -1        8       -1        0        0       -1       -1       -1        0        0        0        0        0        0
       0        0        0        0        0        0        0       -1       -1       -1        0        0       -1        8       -1        0        0       -1       -1       -1        0        0        0        0        0
       0        0        0        0        0        0        0        0       -1       -1        0        0        0       -1        8        0        0        0       -1       -1        0        0        0        0        0
       0        0        0        0        0        0        0        0        0        0       -1       -1        0        0        0        8       -1        0        0        0       -1       -1        0        0        0
       0        0        0        0        0        0        0        0        0        0       -1       -1       -1        0        0       -1        8       -1        0        0       -1       -1       -1        0        0
       0        0        0        0        0        0        0        0        0        0        0       -1       -1       -1        0        0       -1        8       -1        0        0       -1       -1       -1        0
       0        0        0        0        0        0        0        0        0        0        0        0       -1       -1       -1        0        0       -1        8       -1        0        0       -1       -1       -1
       0        0        0        0        0        0        0        0        0        0        0        0        0       -1       -1        0        0        0       -1        8        0        0        0       -1       -1
       0        0        0        0        0        0        0        0        0        0        0        0        0        0        0       -1       -1        0        0        0        8       -1        0        0        0
       0        0        0        0        0        0        0        0        0        0        0        0        0        0        0       -1       -1       -1        0        0       -1        8       -1        0        0
       0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0       -1       -1       -1        0        0       -1        8       -1        0
       0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0       -1       -1       -1        0        0       -1        8       -1
       0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0       -1       -1        0        0        0       -1        8      4       -1        0       -1        0        0        0        0        0
*/

    AMGStandardCoarseningStrengthPolicy strength_policy(m);
    VariableCategorizer variable_categorizer(m.rows());
    VariableInfluenceAccessor influence_accessor(strength_policy, variable_categorizer);
    AMGStandardSplitting splitting(m, influence_accessor, variable_categorizer);
    splitting.generateSplitting();
    variable_categorizer.print();

/*
    F F F F F
    F C F C F
    F F F F F
    F C F C F
    F F F F F
*/

    for (auto variable : {6, 8, 16, 18}) {
        auto variable_type = static_cast<char>(variable_categorizer.GetType(variable));
        auto expected = static_cast<char>(VariableCategorizer::Type::COARSE);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong variable type", expected, variable_type);
    }

    for (auto variable : {0, 1, 2, 3, 4, 5, 7, 9, 10, 11, 12, 13, 14, 15, 17, 19, 20, 21, 22, 23, 24}) {
        auto variable_type = static_cast<char>(variable_categorizer.GetType(variable));
        auto expected = static_cast<char>(VariableCategorizer::Type::FINE);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong variable type", expected, variable_type);
    }
}

void
AMGStandardSplittingTest::TestSplittingForNontrivial9ptStencil() {
    // from Multigrid Tutorial, p. 151, Fig. 8.7
    MatrixStencil stencil;
    stencil << -1, -4, -1,
                2,  8,  2,
               -1, -4, -1;

    SparseMatrix2D const & m = stencil.generateMatrix(7 * 7);

//    m.print();
/*
       8        2        0        0        0        0        0       -4       -1        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0
       2        8        2        0        0        0        0       -1       -4       -1        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0
       0        2        8        2        0        0        0        0       -1       -4       -1        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0
       0        0        2        8        2        0        0        0        0       -1       -4       -1        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0
       0        0        0        2        8        2        0        0        0        0       -1       -4       -1        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0
       0        0        0        0        2        8        2        0        0        0        0       -1       -4       -1        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0
       0        0        0        0        0        2        8        0        0        0        0        0       -1       -4        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0
      -4       -1        0        0        0        0        0        8        2        0        0        0        0        0       -4       -1        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0
      -1       -4       -1        0        0        0        0        2        8        2        0        0        0        0       -1       -4       -1        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0
       0       -1       -4       -1        0        0        0        0        2        8        2        0        0        0        0       -1       -4       -1        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0
       0        0       -1       -4       -1        0        0        0        0        2        8        2        0        0        0        0       -1       -4       -1        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0
       0        0        0       -1       -4       -1        0        0        0        0        2        8        2        0        0        0        0       -1       -4       -1        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0
       0        0        0        0       -1       -4       -1        0        0        0        0        2        8        2        0        0        0        0       -1       -4       -1        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0
       0        0        0        0        0       -1       -4        0        0        0        0        0        2        8        0        0        0        0        0       -1       -4        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0
       0        0        0        0        0        0        0       -4       -1        0        0        0        0        0        8        2        0        0        0        0        0       -4       -1        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0
       0        0        0        0        0        0        0       -1       -4       -1        0        0        0        0        2        8        2        0        0        0        0       -1       -4       -1        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0
       0        0        0        0        0        0        0        0       -1       -4       -1        0        0        0        0        2        8        2        0        0        0        0       -1       -4       -1        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0
       0        0        0        0        0        0        0        0        0       -1       -4       -1        0        0        0        0        2        8        2        0        0        0        0       -1       -4       -1        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0
       0        0        0        0        0        0        0        0        0        0       -1       -4       -1        0        0        0        0        2        8        2        0        0        0        0       -1       -4       -1        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0
       0        0        0        0        0        0        0        0        0        0        0       -1       -4       -1        0        0        0        0        2        8        2        0        0        0        0       -1       -4       -1        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0
       0        0        0        0        0        0        0        0        0        0        0        0       -1       -4        0        0        0        0        0        2        8        0        0        0        0        0       -1       -4        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0
       0        0        0        0        0        0        0        0        0        0        0        0        0        0       -4       -1        0        0        0        0        0        8        2        0        0        0        0        0       -4       -1        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0
       0        0        0        0        0        0        0        0        0        0        0        0        0        0       -1       -4       -1        0        0        0        0        2        8        2        0        0        0        0       -1       -4       -1        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0
       0        0        0        0        0        0        0        0        0        0        0        0        0        0        0       -1       -4       -1        0        0        0        0        2        8        2        0        0        0        0       -1       -4       -1        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0
       0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0       -1       -4       -1        0        0        0        0        2        8        2        0        0        0        0       -1       -4       -1        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0
       0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0       -1       -4       -1        0        0        0        0        2        8        2        0        0        0        0       -1       -4       -1        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0
       0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0       -1       -4       -1        0        0        0        0        2        8        2        0        0        0        0       -1       -4       -1        0        0        0        0        0        0        0        0        0        0        0        0        0        0
       0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0       -1       -4        0        0        0        0        0        2        8        0        0        0        0        0       -1       -4        0        0        0        0        0        0        0        0        0        0        0        0        0        0
       0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0       -4       -1        0        0        0        0        0        8        2        0        0        0        0        0       -4       -1        0        0        0        0        0        0        0        0        0        0        0        0
       0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0       -1       -4       -1        0        0        0        0        2        8        2        0        0        0        0       -1       -4       -1        0        0        0        0        0        0        0        0        0        0        0
       0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0       -1       -4       -1        0        0        0        0        2        8        2        0        0        0        0       -1       -4       -1        0        0        0        0        0        0        0        0        0        0
       0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0       -1       -4       -1        0        0        0        0        2        8        2        0        0        0        0       -1       -4       -1        0        0        0        0        0        0        0        0        0
       0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0       -1       -4       -1        0        0        0        0        2        8        2        0        0        0        0       -1       -4       -1        0        0        0        0        0        0        0        0
       0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0       -1       -4       -1        0        0        0        0        2        8        2        0        0        0        0       -1       -4       -1        0        0        0        0        0        0        0
       0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0       -1       -4        0        0        0        0        0        2        8        0        0        0        0        0       -1       -4        0        0        0        0        0        0        0
       0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0       -4       -1        0        0        0        0        0        8        2        0        0        0        0        0       -4       -1        0        0        0        0        0
       0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0       -1       -4       -1        0        0        0        0        2        8        2        0        0        0        0       -1       -4       -1        0        0        0        0
       0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0       -1       -4       -1        0        0        0        0        2        8        2        0        0        0        0       -1       -4       -1        0        0        0
       0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0       -1       -4       -1        0        0        0        0        2        8        2        0        0        0        0       -1       -4       -1        0        0
       0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0       -1       -4       -1        0        0        0        0        2        8        2        0        0        0        0       -1       -4       -1        0
       0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0       -1       -4       -1        0        0        0        0        2        8        2        0        0        0        0       -1       -4       -1
       0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0       -1       -4        0        0        0        0        0        2        8        0        0        0        0        0       -1       -4
       0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0       -4       -1        0        0        0        0        0        8        2        0        0        0        0        0
       0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0       -1       -4       -1        0        0        0        0        2        8        2        0        0        0        0
       0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0       -1       -4       -1        0        0        0        0        2        8        2        0        0        0
       0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0       -1       -4       -1        0        0        0        0        2        8        2        0        0
       0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0       -1       -4       -1        0        0        0        0        2        8        2        0
       0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0       -1       -4       -1        0        0        0        0        2        8        2
       0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0        0       -1       -4        0        0        0        0        0        2        8*/

    AMGStandardCoarseningStrengthPolicy strength_policy(m);
    VariableCategorizer variable_categorizer(m.rows());
    VariableInfluenceAccessor influence_accessor(strength_policy, variable_categorizer);
    AMGStandardSplitting splitting(m, influence_accessor, variable_categorizer);
    splitting.generateSplitting();
//   variable_categorizer.print();

/*
    F F F F F F F
    C C C C C C C
    F F F F F F F
    C C C C C C C
    F F F F F F F
    C C C C C C C
    F F F F F F F
*/
    for (auto variable : {7, 8, 9, 10, 11, 12, 13, 21, 22, 23, 24, 25, 26, 27, 35, 36, 37, 38, 39, 40, 41}) {
        auto variable_type = static_cast<char>(variable_categorizer.GetType(variable));
        auto expected = static_cast<char>(VariableCategorizer::Type::COARSE);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong variable type", expected, variable_type);
    }

    for (auto variable : {0, 1, 2, 3, 4, 5, 6, 14, 15, 16, 17, 18, 19, 20, 28, 29, 30, 31, 32, 33, 34, 42, 43, 44, 45, 46, 47, 48}) {
        auto variable_type = static_cast<char>(variable_categorizer.GetType(variable));
        auto expected = static_cast<char>(VariableCategorizer::Type::FINE);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong variable type", expected, variable_type);
    }
}
