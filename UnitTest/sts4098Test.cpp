#include "pch.h"

#include "sts4098Test.h"


using namespace EntityReader_NS;
using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;

namespace FS = boost::filesystem;



namespace {

    SparseMatrix2D
    readMatrix() {
        FS::path filename("\\Develop\\SparseMatrixData\\sts4098\\sts4098.ar");
        ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
        CPPUNIT_ASSERT_MESSAGE("File not found", sm_reader);
        CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());
        SparseMatrix2D const m = sm_reader->get();
        CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 4098ull, m.cols());
        return m;
    }

    Vector
    readRHS() {
        FS::path filename = "\\Develop\\SparseMatrixData\\sts4098\\sts4098_b.ar";
        IVectorReader::Ptr b_reader = VectorReaderCreator::create(filename.string());
        CPPUNIT_ASSERT_MESSAGE("error reading vector data", b_reader->read());
        Vector const b = b_reader->get();
        CPPUNIT_ASSERT_EQUAL_MESSAGE("error in vector size", 4098ull, b.size());
        return b;
    }

}

void
sts4098Test::setUp() {
    initMatrix();
    initRHS();
}

void
sts4098Test::initMatrix() {
    m_ = readMatrix();
}

void
sts4098Test::initRHS() {
    b_ = readRHS();
}

void
sts4098Test::tearDown() {}

void
sts4098Test::matrixIsDiagonallyDominant() {
    CPPUNIT_ASSERT_MESSAGE("matrix should not be diagonally dominant", !SparseLinearSolverUtil::isStrictlyDiagonallyDominant(m_));
}

namespace {

    void
    checkThatMatrixTimesVectorYieldsRightHandSide(SparseMatrix2D const & m, Vector const & b, Vector const & x, double tolerance) {
        // compute A x and check that = b
        Vector tmp(x.size());
        tmp = m * x;
        CPPUNIT_ASSERT_MESSAGE("mismatch in linear solver result", SparseLinearSolverUtil::isVectorEqual(tmp, b, tolerance));
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
sts4098Test::SORTest() {
    bool success;
    Vector x(b_.size());
    int iterations;

    {
        HighResTimer t;

        // needs 10986 iterations
        std::tie(success, x, iterations) = SparseLinearSolverLibrary::sparseSOR(m_, b_, 1.1, 11000);
    }
    CPPUNIT_ASSERT_MESSAGE("SOR failed to solve linear system", success);

    checkThatMatrixTimesVectorYieldsRightHandSide(m_, b_, x, 1E-10);
}

void
sts4098Test::MINRESTest() {
    bool success;
    Vector x(b_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 53108 iterations (due to very high condition number?)
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::MINRES(m_, b_, 60000);
    }
    CPPUNIT_ASSERT_MESSAGE("MINRES failed to solve linear system", success);

    // compare vectors
    // Note: The condition number is cond(A) = 2.17087e+8. With a default tolerance
    // of 1E-15 (close to double eps), we cannot expect much more than 1E-7 in the
    // final result.
    checkThatMatrixTimesVectorYieldsRightHandSide(m_, b_, x, 1E-8);
}

void
sts4098Test::MINRESLanProTest() {
    bool success;
    Vector x(b_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 53108 iterations (due to very high condition number?) with reg. MINRES
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::MINRESLanPro(m_, b_, 60000);
    }
    CPPUNIT_ASSERT_MESSAGE("MINRES failed to solve linear system", success);

    // compare vectors
    // Note: The condition number is cond(A) = 2.17087e+8. With a default tolerance
    // of 1E-15 (close to double eps), we cannot expect much more than 1E-7 in the
    // final result.
    checkThatMatrixTimesVectorYieldsRightHandSide(m_, b_, x, 1.3 * 1E-6);
}

void
sts4098Test::GMRESTest() {
    bool success;
    Vector x(b_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 4485 iterations (due to very high condition number?)
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::GMRES(m_, b_, m_.cols());
    }
    CPPUNIT_ASSERT_MESSAGE("GMRES failed to solve linear system", success);

    // compare vectors
    // Note: The condition number is cond(A) = 2.17087e+8. With a default tolerance
    // of 1E-15 (close to double eps), we cannot expect much more than 1E-7 in the
    // final result. However, GMRES performs much better, hence 1E-11!!!
    checkThatMatrixTimesVectorYieldsRightHandSide(m_, b_, x, 1.1 * 1E-11);
}
