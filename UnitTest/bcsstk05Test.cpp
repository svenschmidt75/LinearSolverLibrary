#include "pch.h"

#include "bcsstk05Test.h"

#include "LinAlg/entities.h"
#include "LinAlg/operators.h"

#include "LinearSolverLibrary/ConjugateGradientMethods.hpp"


using namespace EntityReader_NS;
using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;

namespace FS = boost::filesystem;


namespace {

    SparseMatrix2D
    readMatrix() {
        FS::path filename("\\Develop\\SparseMatrixData\\bcsstk05\\bcsstk05.ar");
        ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
        CPPUNIT_ASSERT_MESSAGE("File not found", sm_reader);
        CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());
        SparseMatrix2D const m = sm_reader->get();
        CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 153ull, m.cols());
        return m;
    }

}

void
bcsstk05Test::setUp() {
    initMatrix();

    // create reference solution vector
    Vector x_ref(153);
    std::iota(std::begin(x_ref), std::end(x_ref), 1);
    x_ref_ = x_ref;

    // create rhs
    Vector b(153);
    b = m_ * x_ref;
    b_ = b;
}

void
bcsstk05Test::initMatrix() {
    m_ = readMatrix();
}

void
bcsstk05Test::tearDown() {}

void
bcsstk05Test::matrixIsDiagonallyDominant() {
    CPPUNIT_ASSERT_MESSAGE("matrix not diagonally dominant", !SparseLinearSolverUtil::isStrictlyDiagonallyDominant(m_));
}

void
bcsstk05Test::matrixIsSymmetric() {
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

void
bcsstk05Test::SORTest() {
    // BCSSTK05 is symmetric pos. def., but not diagonally dominant
    bool success;
    Vector x(b_.size());
    int iterations;

    // needs 17687 iterations
    std::tie(success, x, iterations) = SparseLinearSolverLibrary::sparseSOR(m_, b_, 1.1, 20000);

    CPPUNIT_ASSERT_MESSAGE("SOR failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in SOR solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 1E-10));
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
bcsstk05Test::CGTest() {
    // BCSSTK05 is symmetric pos. def., but not diagonally dominant
    bool success;
    Vector x(b_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 321 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::CG(m_, b_, 10000);
    }
    CPPUNIT_ASSERT_MESSAGE("CG failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in CG solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 1E-10));
}

void
bcsstk05Test::BiCGTest() {
    // BCSSTK05 is symmetric pos. def., but not diagonally dominant
    bool success;
    Vector x(b_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 321 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::BiCG(m_, LinAlg_NS::helper::transpose(m_), b_, 10000);
    }
    CPPUNIT_ASSERT_MESSAGE("BiCG failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in BiCG solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 1E-10));
}

void
bcsstk05Test::BiCGSTABTest() {
    // BCSSTK05 is symmetric pos. def., but not diagonally dominant
    bool success;
    Vector x(b_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 355 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::BiCGSTAB(m_, b_, 10000);
    }
    CPPUNIT_ASSERT_MESSAGE("BiCGSTAB failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in BiCGSTAB solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 1E-10));
}

void
bcsstk05Test::MINRESTest() {
    // BCSSTK05 is symmetric pos. def., but not diagonally dominant
    bool success;
    Vector x(b_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 323 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::MINRES(m_, b_, 10000);
    }
    CPPUNIT_ASSERT_MESSAGE("MINRES failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in MINRES solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 1E-10));
}

void
bcsstk05Test::MINRESLanProTest() {
    // BCSSTK05 is symmetric pos. def., but not diagonally dominant
    bool success;
    Vector x(b_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 152 iterations vs 323 without partial reorthogonalization
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::MINRESLanPro(m_, b_, 10000);
    }
    CPPUNIT_ASSERT_MESSAGE("MINRESLanPRO failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in MINRESLanPRO solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 1E-9));
}

void
bcsstk05Test::GCRWithNoRestartTest() {
    // BCSSTK05 is symmetric pos. def., but not diagonally dominant
    bool success;
    Vector x(b_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 152 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::GCR(m_, b_, 10000, 10000);
    }
    CPPUNIT_ASSERT_MESSAGE("GCR failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in GCR solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 1E-9));
}

void
bcsstk05Test::GCRWithRestartTest() {
    // BCSSTK05 is symmetric pos. def., but not diagonally dominant
    bool success;
    Vector x(b_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 670 iterations vs 152 without restart
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::GCR(m_, b_, 100, 10000);
    }
    CPPUNIT_ASSERT_MESSAGE("GCR failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in GCR solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 2 * 1E-9));
}
