#include "pch.h"

#include "FIDAP_EX5_Test.h"


using namespace EntityReader_NS;
using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;

namespace FS = boost::filesystem;


namespace {

    SparseMatrix2D
    readMatrix() {
        FS::path filename(R"(\Develop\SparseMatrixData\FIDAP\EX5\EX5.ar)");
        ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
        CPPUNIT_ASSERT_MESSAGE("File not found", sm_reader);
        CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());
        SparseMatrix2D const m = sm_reader->get();
        CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 27ull, m.rows());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 27ull, m.cols());
        return m;
    }

}

void
FIDAP_EX5_Test::setUp() {
    initMatrix();

    // create reference solution vector
    Vector x_ref(27);
    std::iota(std::begin(x_ref), std::end(x_ref), 1);
    x_ref_ = x_ref;

    // create rhs
    Vector b(27);
    b = m_ * x_ref;
    b_ = b;
}

void
FIDAP_EX5_Test::initMatrix() {
    m_ = readMatrix();
}

void
FIDAP_EX5_Test::tearDown() {}

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
FIDAP_EX5_Test::CGTest() {
    bool success;
    Vector x(b_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 125 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::CG(m_, b_, 10000);
    }
    CPPUNIT_ASSERT_MESSAGE("CG failed to solve linear system", success);

    checkThatMatrixTimesVectorYieldsRightHandSide(m_, b_, x, 3.0 * 1E-8);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in CG solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 6.0 * 1E-9));
}

void
FIDAP_EX5_Test::BiCGTest() {
    bool success;
    Vector x(b_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 125 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::BiCG(m_, LinAlg_NS::helper::transpose(m_), b_, 10000);
    }

    checkThatMatrixTimesVectorYieldsRightHandSide(m_, b_, x, 3.0 * 1E-8);

    CPPUNIT_ASSERT_MESSAGE("BiCG failed to solve linear system", success);
    CPPUNIT_ASSERT_MESSAGE("mismatch in BiCG solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 6.0 * 1E-9));
}

void
FIDAP_EX5_Test::GMRESTest() {
    bool success;
    Vector x(b_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 27 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::GMRES(m_, b_, 680, 10000);
    }

    checkThatMatrixTimesVectorYieldsRightHandSide(m_, b_, x, 2.3 * 1E-8);

    CPPUNIT_ASSERT_MESSAGE("GMRES failed to solve linear system", success);
    CPPUNIT_ASSERT_MESSAGE("mismatch in GMRES solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 6.0 * 1E-9));
}

void
FIDAP_EX5_Test::TFQMRTest() {
    bool success;
    Vector x(b_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // TFQMR cannot solve this system due to breakdown (rho2 = 0 in iteration 55)
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::TFQMR(m_, b_, 10000);
    }
    CPPUNIT_ASSERT_MESSAGE("TFQMR should show breakdown", !success);
}

void
FIDAP_EX5_Test::GCRWithNoRestartTest() {
    bool success;
    Vector x(b_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 26 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::GCR(m_, b_, 170, 30);
    }

    checkThatMatrixTimesVectorYieldsRightHandSide(m_, b_, x, 1.3 * 1E-8);

    CPPUNIT_ASSERT_MESSAGE("GCR failed to solve linear system", success);
    CPPUNIT_ASSERT_MESSAGE("mismatch in GCR solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 9.0 * 1E-9));
}

void
FIDAP_EX5_Test::GCRWithRestartTest() {
    bool success;
    Vector x(b_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 75 iterations, compared to 26, although the restart is 25!!!
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::GCR(m_, b_, 25, 100);
    }

    checkThatMatrixTimesVectorYieldsRightHandSide(m_, b_, x, 5.4 * 1E-8);

    CPPUNIT_ASSERT_MESSAGE("GCR failed to solve linear system", success);
    CPPUNIT_ASSERT_MESSAGE("mismatch in GCR solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 3.0 * 1E-7));
}

void
FIDAP_EX5_Test::MINRESTest() {
    bool success;
    Vector x(b_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 103 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::MINRES(m_, b_, 110);
    }
    CPPUNIT_ASSERT_MESSAGE("MINRES failed to solve linear system", success);

    // compare vectors
    // Note: The condition number is cond(A) = 6.64996e+07, \approx 1e+8. With a
    // default tolerance of 1E-15 (close to double eps), we cannot expect much
    // more than 1E-7 in the final result.
    CPPUNIT_ASSERT_MESSAGE("mismatch in SOR solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 3.0 * 1E-8));

    /* Why is the following comparison so bad?
     * Maybe because of the high condition number. We multiply the resulting
     * vector x by m_ to get b', which we diff with b_. Small error can have
     * a big effect due to the high condition number?
     */
    checkThatMatrixTimesVectorYieldsRightHandSide(m_, b_, x, 4.7 * 1E-2);
}

void
FIDAP_EX5_Test::MINRESLanProTest() {
    bool success;
    Vector x(b_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 26 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::MINRESLanPro(m_, b_, 30);
    }
    CPPUNIT_ASSERT_MESSAGE("MINRES failed to solve linear system", success);

    // compare vectors
    // Note: The condition number is cond(A) = 6.64996e+07, \approx 1e+8. With a
    // default tolerance of 1E-15 (close to double eps), we cannot expect much
    // more than 1E-7 in the final result.
    checkThatMatrixTimesVectorYieldsRightHandSide(m_, b_, x, 2.9 * 1E-4);
    CPPUNIT_ASSERT_MESSAGE("mismatch in SOR solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 7.0 * 1E-9));
}
