#include "pch.h"

#include "AMGVariableCardinalityTest.h"

#include "LinAlg/MatrixStencil.hpp"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


void
AMGVariableCardinalityTest::setUp() {}

void
AMGVariableCardinalityTest::tearDown() {}

void
AMGVariableCardinalityTest::TestCardinalityOfCornerGridVariable() {
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

    VariableCategorizer variable_categorizer(m.rows());
    VariableInfluenceAccessor influence_accessor(strength_policy, variable_categorizer);
    VariableCardinalityPolicy cardinalityPolicy(influence_accessor);

    IVariableSet::size_type variable = 0;
    auto cardinality = cardinalityPolicy.GetCardinalityForVariable(variable);
    decltype(cardinality) expected_cardinality = 2;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong cardinality", expected_cardinality, cardinality);


    variable_categorizer.SetType(1, VariableCategorizer::Type::FINE);
    cardinality = cardinalityPolicy.GetCardinalityForVariable(variable);
    expected_cardinality = 3;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong cardinality", expected_cardinality, cardinality);
}

void
AMGVariableCardinalityTest::TestCardinalityOfBoundaryGridVariable() {
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

    VariableCategorizer variable_categorizer(m.rows());
    VariableInfluenceAccessor influence_accessor(strength_policy, variable_categorizer);
    VariableCardinalityPolicy cardinalityPolicy(influence_accessor);

    IVariableSet::size_type variable = 7;
    auto cardinality = cardinalityPolicy.GetCardinalityForVariable(variable);
    decltype(cardinality) expected_cardinality = 3;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong cardinality", expected_cardinality, cardinality);


    variable_categorizer.SetType(4, VariableCategorizer::Type::FINE);
    cardinality = cardinalityPolicy.GetCardinalityForVariable(variable);
    expected_cardinality = 4;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong cardinality", expected_cardinality, cardinality);
}

void
AMGVariableCardinalityTest::TestCardinalityOfBoundaryGridVariableWithUnrelatedChanges() {
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

    VariableCategorizer variable_categorizer(m.rows());
    VariableInfluenceAccessor influence_accessor(strength_policy, variable_categorizer);
    VariableCardinalityPolicy cardinalityPolicy(influence_accessor);

    IVariableSet::size_type variable = 7;
    auto cardinality = cardinalityPolicy.GetCardinalityForVariable(variable);
    decltype(cardinality) expected_cardinality = 3;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong cardinality", expected_cardinality, cardinality);


    variable_categorizer.SetType(0, VariableCategorizer::Type::FINE);
    variable_categorizer.SetType(3, VariableCategorizer::Type::COARSE);
    variable_categorizer.SetType(5, VariableCategorizer::Type::COARSE);
    cardinality = cardinalityPolicy.GetCardinalityForVariable(variable);
    expected_cardinality = 3;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong cardinality", expected_cardinality, cardinality);
}

void
AMGVariableCardinalityTest::TestCardinalityOfCenterGridVariable() {
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

    VariableCategorizer variable_categorizer(m.rows());
    VariableInfluenceAccessor influence_accessor(strength_policy, variable_categorizer);
    VariableCardinalityPolicy cardinalityPolicy(influence_accessor);

    IVariableSet::size_type variable = 4;
    auto cardinality = cardinalityPolicy.GetCardinalityForVariable(variable);
    decltype(cardinality) expected_cardinality = 4;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong cardinality", expected_cardinality, cardinality);


    variable_categorizer.SetType(1, VariableCategorizer::Type::FINE);
    variable_categorizer.SetType(3, VariableCategorizer::Type::FINE);
    variable_categorizer.SetType(5, VariableCategorizer::Type::FINE);
    cardinality = cardinalityPolicy.GetCardinalityForVariable(variable);
    expected_cardinality = 7;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong cardinality", expected_cardinality, cardinality);
}

void
AMGVariableCardinalityTest::TestThatCardinalityIsSameForAllVariablesWithPeriodicBoundaryConditions() {
    // from Multigrid Tutorial, p. 150, Fig. 8.5
    MatrixStencil<PeriodicBoundaryConditionPolicy> stencil;

    // stencil on page 147, equ. 8.14
    stencil << -1, -1, -1,
               -1,  8, -1,
               -1, -1, -1;

    SparseMatrix2D const & m = stencil.generateMatrix(7 * 7);
//    m.print();


    AMGStandardStrengthPolicy strength_policy(m);
    VariableCategorizer variable_categorizer(m.rows());
    VariableInfluenceAccessor influence_accessor(strength_policy, variable_categorizer);
    VariableCardinalityPolicy cardinalityPolicy(influence_accessor);

    auto matrix_dim = 7;
    Matrix2D coeff(matrix_dim, matrix_dim);
    for (int i = 0; i < matrix_dim; ++i) {
        for (int j = 0; j < matrix_dim; ++j) {
            auto variable = i * matrix_dim + j;
            auto cardinality = cardinalityPolicy.GetCardinalityForVariable(variable);
            decltype(cardinality) expected_cardinality = 8;
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong cardinality", expected_cardinality, cardinality);
        }
    }
}
