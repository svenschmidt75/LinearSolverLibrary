#include "pch.h"

#include "spiralTest.h"

#include "LinAlg/BasicEntityOperators.h"
#include "LinAlg/operators.h"

#include "LinearSolverLibrary/ConjugateGradientMethods.hpp"


using namespace EntityReader_NS;
using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;

namespace FS = boost::filesystem;


namespace {

    SparseMatrix2D
    readMatrix() {
        FS::path filename("\\Develop\\SparseMatrixData\\spiral\\spiral.ar");
        ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
        CPPUNIT_ASSERT_MESSAGE("File not found", sm_reader);
        CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());
        SparseMatrix2D const m = sm_reader->get();
        CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 1434ull, m.cols());
        return m;
    }

}

void
spiralTest::setUp() {
    initMatrix();

    // create reference solution vector
    Vector x_ref(1434);
    std::iota(std::begin(x_ref), std::end(x_ref), 1);
    x_ref_ = x_ref;

    // create rhs
    Vector b(1434);
    b = m_ * x_ref;
    b_ = b;
}

void
spiralTest::initMatrix() {
    m_ = readMatrix();
}

void
spiralTest::tearDown() {}

void
spiralTest::matrixIsDiagonallyDominant() {
    CPPUNIT_ASSERT_MESSAGE("matrix not diagonally dominant", !SparseLinearSolverUtil::isStrictlyDiagonallyDominant(m_));
}

void
spiralTest::matrixIsSymmetric() {
    CPPUNIT_ASSERT_MESSAGE("Matrix not symmetric", LinAlg_NS::helper::isSymmetric(m_));
}

namespace {

    void
    checkThatMatrixTimesVectorYieldsRightHandSide(SparseMatrix2D const & m, Vector const & b, Vector const & x) {
        // compute A x and check that = b
        Vector tmp(x.size());
        tmp = m * x;
        CPPUNIT_ASSERT_MESSAGE("mismatch in linear solver result", SparseLinearSolverUtil::isVectorEqual(tmp, b, 1E-10));
    }

}

namespace {

    class HighResTimer {
    public:
        HighResTimer() : start_(boost::chrono::high_resolution_clock::now()) {}
        ~HighResTimer() {
            auto end = boost::chrono::high_resolution_clock::now();
            auto d = boost::chrono::duration_cast<boost::chrono::milliseconds>(end - start_);
            std::cout << std::endl << "Duration: " << d << std::endl;
        }

    private:
        boost::chrono::steady_clock::time_point start_;
    };

}

void
spiralTest::spiralMINRESTest() {
    // spiral is symmetric, not pos. def.
    // condition nr: 390841
    bool success;
    Vector x(b_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 1989 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::MINRES(m_, b_, 10000);
    }

    CPPUNIT_ASSERT_MESSAGE("MINRES failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in MINRES solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 2.56 * 1E-8));
}

void
spiralTest::spiralMINRESLanProTest() {
    // spiral is symmetric, not pos. def.
    // condition nr: 390841
    bool success;
    Vector x(b_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 634 iterations vs 1989 without partial reorthogonalization
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::MINRESLanPro(m_, b_, 10000);
    }

    CPPUNIT_ASSERT_MESSAGE("MINRESLanPRO failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in MINRESLanPRO solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 9.8 * 1E-7));
}

void
spiralTest::spiralGMRESTest() {
    // spiral is symmetric, not pos. def.
    // condition nr: 390841
    bool success;
    Vector x(b_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 1061 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::GMRES(m_, b_, 1200, 10000);
    }

    CPPUNIT_ASSERT_MESSAGE("GMRES failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in GMRES solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 1E-8));
}

void
spiralTest::GCRWithNoRestartTest() {
    bool success;
    Vector x(b_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 616 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::GCR(m_, b_, 680, 10000);
    }
    CPPUNIT_ASSERT_MESSAGE("GCR failed to solve linear system", success);
    CPPUNIT_ASSERT_MESSAGE("mismatch in GCR solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 5 * 1E-7));
}
