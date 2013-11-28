#include "pch.h"

#include "VersteegMalalasekeraTest.h"

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
        FS::path filename("\\Develop\\SparseMatrixData\\Versteeg_Malalasekera\\Versteeg_Malalasekera.ar");
        ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
        CPPUNIT_ASSERT_MESSAGE("File not found", sm_reader);
        CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());
        SparseMatrix2D const m = sm_reader->get();
        CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 18ull, m.cols());
        return m;
    }

    Vector
    readRHS() {
        FS::path filename = "\\Develop\\SparseMatrixData\\Versteeg_Malalasekera\\Versteeg_Malalasekera_b.ar";
        IVectorReader::Ptr b_reader = VectorReaderCreator::create(filename.string());
        CPPUNIT_ASSERT_MESSAGE("error reading vector data", b_reader->read());
        Vector const b = b_reader->get();
        CPPUNIT_ASSERT_EQUAL_MESSAGE("error in vector size", 18ull, b.size());
        return b;
    }

    Vector
    readReferenceVector() {
        FS::path filename = "\\Develop\\SparseMatrixData\\Versteeg_Malalasekera\\Versteeg_Malalasekera_x.ar";
        IVectorReader::Ptr x_reader = VectorReaderCreator::create(filename.string());
        CPPUNIT_ASSERT_MESSAGE("error reading vector data", x_reader->read());
        Vector const x_ref = x_reader->get();
        CPPUNIT_ASSERT_EQUAL_MESSAGE("error in vector size", 18ull, x_ref.size());
        return x_ref;
    }

}

void
VersteegMalalasekeraTest::setUp() {
    initMatrix();
    initRHS();
    initReferenceVector();
}

void
VersteegMalalasekeraTest::initMatrix() {
    m_ = readMatrix();
}

void
VersteegMalalasekeraTest::initRHS() {
    b_ = readRHS();
}

void
VersteegMalalasekeraTest::initReferenceVector() {
    x_ref_ = readReferenceVector();
}

void
VersteegMalalasekeraTest::tearDown() {}

void
VersteegMalalasekeraTest::matrixIsDiagonallyDominant() {
    CPPUNIT_ASSERT_MESSAGE("matrix not diagonally dominant", SparseLinearSolverUtil::isStrictlyDiagonallyDominant(m_));
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
VersteegMalalasekeraTest::SORTest() {
    bool success;
    Vector x(x_ref_.size());
    int iterations;

    // needs 33 iterations
    std::tie(success, x, iterations) = SparseLinearSolverLibrary::sparseSOR(m_, b_, 1.1, 10000);
    CPPUNIT_ASSERT_MESSAGE("SOR failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in SOR solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 1E-10));

    checkThatMatrixTimesVectorYieldsRightHandSide(m_, b_, x);
}

void
VersteegMalalasekeraTest::SORMultiColorTest() {
    // compute multi-color decomposition
    MatrixDecomposition m_decomp = SparseLinearSolverUtil::multicolorDecomposition(m_);

//    m.print();

    bool success;
    Vector x(x_ref_.size());
    int iterations;

    // needs 32 iterations
    std::tie(success, x, iterations) = SparseLinearSolverLibrary::sparseSORMultiColor(m_, b_, m_decomp, 1.1, 10000);
    CPPUNIT_ASSERT_MESSAGE("SOR failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in SOR solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 1E-10));

    checkThatMatrixTimesVectorYieldsRightHandSide(m_, b_, x);
}

void
VersteegMalalasekeraTest::CGTest() {
    bool success;
    Vector x(x_ref_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    // needs 4 iterations
    std::tie(success, x, iterations, tol) = ConjugateGradientMethods::CG(m_, b_, 10000);
    CPPUNIT_ASSERT_MESSAGE("CG failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in CG solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 1E-10));

    checkThatMatrixTimesVectorYieldsRightHandSide(m_, b_, x);
}

void
VersteegMalalasekeraTest::BiCGTest() {
    bool success;
    Vector x(x_ref_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    // - calling transpose here is unnecessary, as m is symmetric
    // - this is why BiCG is no better than CG

    // needs 4 iterations
    std::tie(success, x, iterations, tol) = ConjugateGradientMethods::BiCG(m_, LinAlg_NS::helper::transpose(m_), b_, 10000);
    CPPUNIT_ASSERT_MESSAGE("BiCG failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in BiCG solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 1E-10));

    checkThatMatrixTimesVectorYieldsRightHandSide(m_, b_, x);
}

void
VersteegMalalasekeraTest::BiCGSTABTest() {
    bool success;
    Vector x(x_ref_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    // needs 4 iterations
    std::tie(success, x, iterations, tol) = ConjugateGradientMethods::BiCGSTAB(m_, b_, 10000);
    CPPUNIT_ASSERT_MESSAGE("BiCGSTAB failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in BiCGSTAB solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 1E-10));

    checkThatMatrixTimesVectorYieldsRightHandSide(m_, b_, x);
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
VersteegMalalasekeraTest::GMRESWithNoRestartsTest() {
    bool success;
    Vector x(x_ref_.size());
    SparseMatrix2D::size_type iterations;
    double tol;


    /************************************************************************/
    /* Demonstrate the ideal case of iterations needed = 5                  */
    /************************************************************************/
    {
        HighResTimer t;

        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::GMRES(m_, b_, 10, 10000);
    }
    CPPUNIT_ASSERT_MESSAGE("GMRES failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in GMRES solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 1E-10));

    checkThatMatrixTimesVectorYieldsRightHandSide(m_, b_, x);
}

void
VersteegMalalasekeraTest::GMRESWithRestartsTest() {
    bool success;
    Vector x(x_ref_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    /************************************************************************/
    /* Demonstrate what happens if restart too small                        */
    /************************************************************************/

    {
        HighResTimer t;

        // restart after 4 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::GMRES(m_, b_, 4, 10000);
    }

    // needs 28(!!!) iterations
    CPPUNIT_ASSERT_MESSAGE("GMRES failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in GMRES solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 1E-10));

    checkThatMatrixTimesVectorYieldsRightHandSide(m_, b_, x);
}

void
VersteegMalalasekeraTest::MINRESTest() {
    bool success;
    Vector x(x_ref_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 4 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::MINRES(m_, b_, 10000);
    }
    CPPUNIT_ASSERT_MESSAGE("MINRES failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in MINRES solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 1E-10));

    checkThatMatrixTimesVectorYieldsRightHandSide(m_, b_, x);
}

void
VersteegMalalasekeraTest::MINRESLanProTest() {
    bool success;
    Vector x(x_ref_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 4 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::MINRESLanPro(m_, b_, 10000);
    }
    CPPUNIT_ASSERT_MESSAGE("MINRES failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in MINRES solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 1E-10));

    checkThatMatrixTimesVectorYieldsRightHandSide(m_, b_, x);
}

void
VersteegMalalasekeraTest::TFQMRTest() {
    bool success;
    Vector x(x_ref_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 4 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::TFQMR(m_, b_, 10000);
    }
    CPPUNIT_ASSERT_MESSAGE("TFQMR failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in TFQMR solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 1E-10));

    checkThatMatrixTimesVectorYieldsRightHandSide(m_, b_, x);
}

void
VersteegMalalasekeraTest::GCRWithNoRestartTest() {
    bool success;
    Vector x(x_ref_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    /************************************************************************/
    /* Demonstrate what happens if restart too small                        */
    /************************************************************************/

    {
        HighResTimer t;

        // restart after 4 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::GCR(m_, b_, 20, 10000);
    }

    // needs 4 iterations
    CPPUNIT_ASSERT_MESSAGE("GCR failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in GCR solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 1E-10));

    checkThatMatrixTimesVectorYieldsRightHandSide(m_, b_, x);
}

void
VersteegMalalasekeraTest::GCRWithRestartTest() {
    bool success;
    Vector x(x_ref_.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    /************************************************************************/
    /* Demonstrate what happens if restart too small                        */
    /************************************************************************/

    {
        HighResTimer t;

        // restart after 27 (!!!) iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::GCR(m_, b_, 4, 10000);
    }

    // needs 27 iterations
    CPPUNIT_ASSERT_MESSAGE("GCR failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in GCR solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref_, 1E-10));

    checkThatMatrixTimesVectorYieldsRightHandSide(m_, b_, x);
}
