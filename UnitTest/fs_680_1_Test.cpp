#include "pch.h"

#include "fs_680_1_Test.h"

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
        FS::path filename("\\Develop\\SparseMatrixData\\fs_680_1\\fs_680_1.ar");
        ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
        CPPUNIT_ASSERT_MESSAGE("File not found", sm_reader);
        CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());
        SparseMatrix2D const m = sm_reader->get();
        CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 680ull, m.cols());
        return m;
    }

}

void
fs_680_1_Test::setUp() {
    initMatrix();

    // create reference solution vector
    Vector x_ref(680);
    std::iota(std::begin(x_ref), std::end(x_ref), 1);
    x_ref_ = x_ref;

    // create rhs
    Vector b(680);
    b = m_ * x_ref;
    b_ = b;
}

void
fs_680_1_Test::initMatrix() {
    m_ = readMatrix();
}

void
fs_680_1_Test::tearDown() {}

void
fs_680_1_Test::matrixIsDiagonallyDominant() {
    CPPUNIT_ASSERT_MESSAGE("matrix not diagonally dominant", !SparseLinearSolverUtil::isStrictlyDiagonallyDominant(m_));
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
fs_680_1_Test::CGTest() {
    bool success;
    Vector x(b_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // CG cannot solve this matrix as it is not symmetric
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::CG(m_, b_, 1000);
    }
    CPPUNIT_ASSERT_MESSAGE("CG should have failed to solve linear system", !success);
}

void
fs_680_1_Test::BiCGTest() {
    bool success;
    Vector x(b_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 533 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::BiCG(m_, LinAlg_NS::helper::transpose(m_), b_, 10000);
    }
    CPPUNIT_ASSERT_MESSAGE("BiCG failed to solve linear system", success);
    CPPUNIT_ASSERT_MESSAGE("mismatch in BiCG solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 1E-8));
}

void
fs_680_1_Test::BiCGSTABTest() {
    bool success;
    Vector x(b_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 419 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::BiCGSTAB(m_, b_, 10000);
    }
    CPPUNIT_ASSERT_MESSAGE("BiCGSTAB failed to solve linear system", success);
    CPPUNIT_ASSERT_MESSAGE("mismatch in BiCGSTAB solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 1E-8));
}

void
fs_680_1_Test::GMRESTest() {
    bool success;
    Vector x(b_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 282 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::GMRES(m_, b_, 680, 10000);
    }
    CPPUNIT_ASSERT_MESSAGE("GMRES failed to solve linear system", success);
    CPPUNIT_ASSERT_MESSAGE("mismatch in GMRES solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 1E-8));
}

void
fs_680_1_Test::TFQMRTest() {
    bool success;
    Vector x(b_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 517 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::TFQMR(m_, b_, 10000);
    }
    CPPUNIT_ASSERT_MESSAGE("TFQMR failed to solve linear system", success);
    CPPUNIT_ASSERT_MESSAGE("mismatch in TFQMR solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 1E-7));
}

void
fs_680_1_Test::GCRWithNoRestartTest() {
    bool success;
    Vector x(b_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 141 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::GCR(m_, b_, 170, 10000);
    }
    CPPUNIT_ASSERT_MESSAGE("GCR failed to solve linear system", success);
    CPPUNIT_ASSERT_MESSAGE("mismatch in GCR solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 1E-9));
}

void
fs_680_1_Test::GCRWithRestartTest() {
    bool success;
    Vector x(b_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 220 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::GCR(m_, b_, 200, 10000);
    }
    CPPUNIT_ASSERT_MESSAGE("GCR failed to solve linear system", success);
    CPPUNIT_ASSERT_MESSAGE("mismatch in GCR solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 1E-7));
}
