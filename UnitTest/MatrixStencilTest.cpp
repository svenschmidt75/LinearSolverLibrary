#include "pch.h"

#include "MatrixStencilTest.h"

#include "LinAlg/MatrixStencil.hpp"
#include "LinAlg/DirichletBoundaryConditionPolicy.hpp"
#include "LinAlg/PeriodicBoundaryConditionPolicy.hpp"
#include "LinAlg/iterators.h"


using namespace LinAlg_NS;


void
MatrixStencilTest::setUp() {}

void
MatrixStencilTest::tearDown() {}

void
MatrixStencilTest::TestInitialization() {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil << -1, 3, -17.5;
    double expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil error", expected, stencil.values_[0]);
    expected = 3;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil error", expected, stencil.values_[1]);
    expected = -17.5;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil error", expected, stencil.values_[2]);
}

void
MatrixStencilTest::TestIndexTo2DMapping() {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil << -1, -1, -1,
               -1,  8, -1,
               -1, -1, -1;

    short expected = 0;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil index mapping error", expected, std::get<0>(stencil.mapIndexToStencilCoordinates(4)));
    expected = 0;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil index mapping error", expected, std::get<1>(stencil.mapIndexToStencilCoordinates(4)));

    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil index mapping error", expected, std::get<0>(stencil.mapIndexToStencilCoordinates(0)));
    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil index mapping error", expected, std::get<1>(stencil.mapIndexToStencilCoordinates(0)));

    expected = -1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil index mapping error", expected, std::get<0>(stencil.mapIndexToStencilCoordinates(6)));
    expected = 1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil index mapping error", expected, std::get<1>(stencil.mapIndexToStencilCoordinates(6)));

    expected = 1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil index mapping error", expected, std::get<0>(stencil.mapIndexToStencilCoordinates(5)));
    expected = 0;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil index mapping error", expected, std::get<1>(stencil.mapIndexToStencilCoordinates(5)));
}

void
MatrixStencilTest::Test2DToIndexMapping() {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil << -1, -1, -1,
               -1,  8, -1,
               -1, -1, -1;

    unsigned short expected = 4;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil index mapping error", expected, stencil.mapStencilCoordinatesToIndex(0, 0));

    expected = 0;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil index mapping error", expected, stencil.mapStencilCoordinatesToIndex(-1, -1));

    expected = 3;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil index mapping error", expected, stencil.mapStencilCoordinatesToIndex(-1, 0));

    expected = 7;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stencil index mapping error", expected, stencil.mapStencilCoordinatesToIndex(0, 1));
}

void
MatrixStencilTest::TestThatThrowsOnEvenStencilDimension() {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil << -1, -1, -1,
               -1,  8, -1;

    CPPUNIT_ASSERT_THROW_MESSAGE("Matrix stencil must have odd dimension", stencil.generateMatrix(5), std::runtime_error);
}

void
MatrixStencilTest::TestThatThrowsOnInconsistentStencilDimension() {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil << -1, -1, -1,
               -1, 8, -1,
               -1;

    CPPUNIT_ASSERT_THROW_MESSAGE("Matrix stencil must be of type n x n", stencil.generateMatrix(5), std::runtime_error);
}

namespace {

    bool almostEqual(double d1, double d2, double tol = 1E-3) {
        if (d1 == 0.0)
            return true;
        double diff = std::fabs((d1 - d2) / d1);
        bool success = diff < tol;
        return success;
    }

    template<typename T, typename U>
    void compareMatrices(T const & m1, U const & m2) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Matrices not compatible", m1.cols(), m2.cols());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Matrices not compatible", m1.rows(), m2.rows());
        for (IMatrix2D::size_type i = 0; i < m1.rows(); ++i) {
            for (IMatrix2D::size_type j = 0; j < m1.cols(); ++j) {
                auto m1_value = m1(i, j);
                auto m2_value = m2(i, j);
                CPPUNIT_ASSERT_MESSAGE("Element mismatch", almostEqual(m1_value, m2_value));
            }
        }
    }

}

void
MatrixStencilTest::TestGenerated3By3MatrixForFivePointStencil() {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil <<  0, -1,  0,
               -1,  4, -1,
                0, -1,  0;

    SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);

    Matrix2D reference_matrix(9, 9);
    reference_matrix <<
         4, -1,  0, -1,  0,  0,  0,  0,  0,
        -1,  4, -1,  0, -1,  0,  0,  0,  0,
         0, -1,  4,  0,  0, -1,  0,  0,  0,
        -1,  0,  0,  4, -1,  0, -1,  0,  0,
         0, -1,  0, -1,  4, -1,  0, -1,  0,
         0,  0, -1,  0, -1,  4,  0,  0, -1,
         0,  0,  0, -1,  0,  0,  4, -1,  0,
         0,  0,  0,  0, -1,  0, -1,  4, -1,
         0,  0,  0,  0,  0, -1,  0, -1,  4;

    compareMatrices(m, reference_matrix);
}

void
MatrixStencilTest::TestGenerated3By3MatrixForNinePointStencil() {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil << -1, -4, -1,
               -4, 20, -4,
               -1, -4, -1;

    SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);

    Matrix2D reference_matrix(9, 9);
    reference_matrix <<
        20, -4,  0, -4, -1,  0,  0,  0,  0,
        -4, 20, -4, -1, -4, -1,  0,  0,  0,
         0, -4, 20,  0, -1, -4,  0,  0,  0,
        -4, -1,  0, 20, -4,  0, -4, -1,  0,
        -1, -4, -1, -4, 20, -4, -1, -4, -1,
         0, -1, -4,  0, -4, 20,  0, -1, -4,
         0, -4,  0, -4, -1,  0, 20, -4,  0,
         0, -4,  0, -1, -4, -1, -4, 20, -4,
         0, -4,  0,  0, -1, -4,  0, -4, 20;

    compareMatrices(m, reference_matrix);
}

void
MatrixStencilTest::TestGenerated3By3MatrixFor25PointStencil() {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil <<
         0,  0, -1,  0,  0,
         0,  0, -4,  0,  0,
        -1, -4, 20, -4, -1,
         0,  0, -4,  0,  0,
         0,  0, -1,  0,  0;

    SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);

    Matrix2D reference_matrix(9, 9);
    reference_matrix <<
        20, -4, -1, -4,  0,  0, -1,  0,  0,
        -4, 20, -4,  0, -4,  0,  0, -1,  0,
        -1, -4, 20,  0,  0, -4,  0,  0, -1,
        -4,  0,  0, 20, -4, -1, -4,  0,  0,
         0, -4,  0, -4, 20, -4,  0, -4,  0,
         0,  0, -4, -1, -4, 20,  0,  0, -4,
        -1,  0,  0, -4,  0,  0, 20, -4, -1,
         0, -1,  0,  0, -4,  0, -4, 20, -4,
         0,  0, -1,  0,  0, -4, -1, -4, 20;

    compareMatrices(m, reference_matrix);
}

void
MatrixStencilTest::TestGenerated4By4MatrixFor25PointStencil() {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil << 
         2, -1,  9,  2,  1,
        -1,  4, -1, -6, -3,
         7, -1,  3, -7, -8,
         3,  5, -8, -9, -3,
         0,  1, -2,  7,  2;

    SparseMatrix2D const & m = stencil.generateMatrix(4 * 4);

    Matrix2D reference_matrix(16, 16);
    reference_matrix <<
         3, -7, -8,  0, -8, -9, -3,  0, -2,  7,  2,  0,  0,  0,  0,  0,
        -1,  3, -7, -8,  5, -8, -9, -3,  1, -2,  7,  2,  0,  0,  0,  0,
         7, -1,  3, -7,  3,  5, -8, -9,  0,  1, -2,  7,  0,  0,  0,  0,
         0,  7, -1,  3,  0,  3,  5, -8,  0,  0,  1, -2,  0,  0,  0,  0,
        -1, -6, -3,  0,  3, -7, -8,  0, -8, -9, -3,  0, -2,  7,  2,  0,
         4, -1, -6, -3, -1,  3, -7, -8,  5, -8, -9, -3,  1, -2,  7,  2,
        -1,  4, -1, -6,  7, -1,  3, -7,  3,  5, -8, -9,  0,  1, -2,  7,
         0, -1,  4, -1,  0,  7, -1,  3,  0,  3,  5, -8,  0,  0,  1, -2,
         9,  2,  1,  0, -1, -6, -3,  0,  3, -7, -8,  0, -8, -9, -3,  0,
        -1,  9,  2,  1,  4, -1, -6, -3, -1,  3, -7, -8,  5, -8, -9, -3,
         2, -1,  9,  2, -1,  4, -1, -6,  7, -1,  3, -7,  3,  5, -8, -9,
         0,  2, -1,  9,  0, -1,  4, -1,  0,  7, -1,  3,  0,  3,  5, -8,
         0,  0,  0,  0,  9,  2,  1,  0, -1, -6, -3,  0,  3, -7, -8,  0,
         0,  0,  0,  0, -1,  9,  2,  1,  4, -1, -6, -3, -1,  3, -7, -8,
         0,  0,  0,  0,  2, -1,  9,  2, -1,  4, -1, -6,  7, -1,  3, -7,
         0,  0,  0,  0,  0,  2, -1,  9,  0, -1,  4, -1,  0,  7, -1,  3;

    compareMatrices(m, reference_matrix);
}

void
MatrixStencilTest::TestGenerated3By3MatrixForFivePointStencilWithPeriodicBoundaryConditions() {
    MatrixStencil<PeriodicBoundaryConditionPolicy> stencil;
    stencil << 0, -1,  0,
              -1,  4, -1,
               0, -1,  0;

    SparseMatrix2D const & m = stencil.generateMatrix(3 * 3);

    Matrix2D reference_matrix(9, 9);
    reference_matrix <<
         4, -1,  0, -1,  0,  0,  0,  0,  0,
        -1,  4, -1,  0, -1,  0,  0,  0,  0,
        -1, -1,  4,  0,  0, -1,  0,  0,  0,
        -1,  0,  0,  4, -1,  0, -1,  0,  0,
         0, -1,  0, -1,  4, -1,  0, -1,  0,
         0,  0, -1, -1, -1,  4,  0,  0, -1,
        -1,  0,  0, -1,  0,  0,  4, -1,  0,
         0, -1,  0,  0, -1,  0, -1,  4, -1,
         0,  0, -1,  0,  0, -1, -1, -1,  4;

    compareMatrices(m, reference_matrix);
}

void
MatrixStencilTest::TestGenerated3By3MatrixFor25PointStencilWithPeriodicBoundaryConditions() {
    MatrixStencil<PeriodicBoundaryConditionPolicy> stencil;
    stencil <<
         0,  0, -1,  0,  0,
         0,  0, -4,  0,  0,
        -1, -4, 20, -4, -1,
         0,  0, -4,  0,  0,
         0,  0, -1,  0,  0;

    CPPUNIT_ASSERT_THROW(stencil.generateMatrix(3 * 3), std::runtime_error);
}
void
MatrixStencilTest::TestGenerated5By5MatrixFor25PointStencilWithPeriodicBoundaryConditions() {
    MatrixStencil<PeriodicBoundaryConditionPolicy> stencil;
    stencil <<
         2, -1,  9,  2,  1,
        -1,  4, -1, -6, -3,
         7, -1,  3, -7, -8,
         3,  5, -8, -9, -3,
         0,  1, -2,  7,  2;

    SparseMatrix2D const & m = stencil.generateMatrix(5 * 5);

    Matrix2D reference_matrix(25, 25);
    reference_matrix <<
        3, -7, -8,  0,  0, -8, -9, -3,  0,  0, -2,  7,  2,  0,  0,  0,  0,   0,  0,  0,  0,  0,   0,  0,  0,
       -1,  3, -7, -8,  0,  5, -8, -9, -3,  0,  1, -2,  7,  2,  0,  0,  0,   0,  0,  0,  0,  0,   0,  0,  0,
        7, -1,  3, -7, -8,  3,  5, -8, -9, -3,  0,  1, -2,  7,  2,  0,  0,   0,  0,  0,  0,  0,   0,  0,  0,
       -8,  7, -1,  3, -7, -3,  3,  5, -8, -9,  2,  0,  1, -2,  7,  0,  0,   0,  0,  0,  0,  0,   0,  0,  0,
       -7,  0,  7, -1,  3, -9,  0,  3,  5, -8,  7,  0,  0,  1, -2,  0,  0,   0,  0,  0,  0,  0,   0,  0,  0,
       -1, -6, -3,  0,  0,  3, -7, -8,  0,  0, -8, -9, -3,  0,  0, -2,  7,   2,  0,  0,  0,  0,   0,  0,  0,
        4, -1, -6, -3,  0, -1,  3, -7, -8,  0,  5, -8, -9, -3,  0,  1, -2,   7,  2,  0,  0,  0,   0,  0,  0,
       -1,  4, -1, -6, -3,  7, -1,  3, -7, -8,  3,  5, -8, -9, -3,  0,  1,  -2,  7,  2,  0,  0,   0,  0,  0,
       -3, -1,  4, -1, -6, -8,  7, -1,  3, -7, -3,  3,  5, -8, -9,  2,  0,   1, -2,  7,  0,  0,   0,  0,  0,
       -6,  0, -1,  4, -1, -7,  0,  7, -1,  3, -9,  0,  3,  5, -8,  7,  0,   0,  1, -2,  0,  0,   0,  0,  0,
        9,  2,  1,  0,  0, -1, -6, -3,  0,  0,  3, -7, -8,  0,  0, -8, -9,  -3,  0,  0, -2,  7,   2,  0,  0,
       -1,  9,  2,  1,  0,  4, -1, -6, -3,  0, -1,  3, -7, -8,  0,  5, -8,  -9, -3,  0,  1, -2,   7,  2,  0,
        2, -1,  9,  2,  1, -1,  4, -1, -6, -3,  7, -1,  3, -7, -8,  3,  5,  -8, -9, -3,  0,  1,  -2,  7,  2,
        1,  2, -1,  9,  2, -3, -1,  4, -1, -6, -8,  7, -1,  3, -7, -3,  3,   5, -8, -9,  2,  0,   1, -2,  7,
        2,  0,  2, -1,  9, -6,  0, -1,  4, -1, -7,  0,  7, -1,  3, -9,  0,   3,  5, -8,  7,  0,   0,  1, -2,
       -2,  7,  2,  0,  0,  9,  2,  1,  0,  0, -1, -6, -3,  0,  0,  3, -7,  -8,  0,  0, -8, -9,  -3,  0,  0,
        1, -2,  7,  2,  0, -1,  9,  2,  1,  0,  4, -1, -6, -3,  0, -1,  3,  -7, -8,  0,  5, -8,  -9, -3,  0,
        0,  1, -2,  7,  2,  2, -1,  9,  2,  1, -1,  4, -1, -6, -3,  7, -1,   3, -7, -8,  3,  5,  -8, -9, -3,
        2,  0,  1, -2,  7,  1,  2, -1,  9,  2, -3, -1,  4, -1, -6, -8,  7,  -1,  3, -7, -3,  3,   5, -8, -9,
        7,  0,  0,  1, -2,  2,  0,  2, -1,  9, -6,  0, -1,  4, -1, -7,  0,   7, -1,  3, -9,  0,   3,  5, -8,
       -8, -9, -3,  0,  0,  0,  0,  0,  0,  0,  9,  2,  1,  0,  0, -1, -6,  -3,  0,  0,  3, -7,  -8,  0,  0,
        5, -8, -9, -3,  0,  0,  0,  0,  0,  0, -1,  9,  2,  1,  0,  4, -1,  -6, -3,  0, -1,  3,  -7, -8,  0,
        3,  5, -8, -9, -3,  0,  0,  0,  0,  0,  2, -1,  9,  2,  1, -1,  4,  -1, -6, -3,  7, -1,   3, -7, -8,
       -3,  3,  5, -8, -9,  0,  0,  0,  0,  0,  1,  2, -1,  9,  2, -3, -1,   4, -1, -6, -8,  7,  -1,  3, -7,
       -9,  0,  3,  5, -8,  0,  0,  0,  0,  0,  2,  0,  2, -1,  9, -6,  0,  -1,  4, -1, -7,  0,   7, -1,  3;
    compareMatrices(m, reference_matrix);
}
