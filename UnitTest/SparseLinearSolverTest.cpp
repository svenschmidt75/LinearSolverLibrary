#include "pch.h"

#include "SparseLinearSolverTest.h"

#include "LinAlg/entities.h"
#include "LinAlg/operators.h"

#include "LinearSolverLibrary/ConjugateGradientMethods.hpp"


using namespace EntityReader_NS;
using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;

namespace FS = boost::filesystem;


void
SparseLinearSolverTest::setUp() {}

void
SparseLinearSolverTest::tearDown() {}


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
SparseLinearSolverTest::SimonExample1Test() {
    /* Example 1 from
     * Horst D Simon, The Lanczos Algorithm With Partial Reorthogonalization
     * page 129
     */

    Vector x_ref(1000);
    SparseMatrix2D m(1000);
    for (IMatrix2D::size_type i = 1; i <= 1000; ++i) {
        double ii = 1E4 * 1.0 / i;
        m(i - 1, i - 1) = ii;
        x_ref(i - 1) = 0.1;
    }
    m.finalize();

    // create vector b
    Vector b(1000);
    b = m * x_ref;



    bool success;
    Vector x(b.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 103 iterations vs 186 without partial reorthogonalization
//        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::MINRES(m, b, 10000);
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::MINRESLanPro(m, b, 10000);
    }

    CPPUNIT_ASSERT_MESSAGE("MINRESLanPRO failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in MINRESLanPRO solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref, 1E-9));
}

void
SparseLinearSolverTest::OffshoreSORTest() {
    // read matrix m
    FS::path filename("\\Develop\\SparseMatrixData\\offshore\\offshore.ar");
    ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("File not found", sm_reader);
    CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());

    SparseMatrix2D const m = sm_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 259789ull, m.cols());


    // check m is NOT diagonally dominant
    CPPUNIT_ASSERT_MESSAGE("matrix diagonally dominant", !SparseLinearSolverUtil::isStrictlyDiagonallyDominant(m));



    // read rhs vector b
    filename = "\\Develop\\SparseMatrixData\\offshore\\offshore_b.ar";
    IVectorReader::Ptr b_reader = VectorReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading vector data", b_reader->read());

    Vector const b = b_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in vector size", 259789ull, b.size());



    bool success;
    Vector x(b.size());
    int iterations;

    {
        HighResTimer t;

        // needs 15 iterations
        std::tie(success, x, iterations) = SparseLinearSolverLibrary::sparseSOR(m, b, 1.1, 20);
    }

    CPPUNIT_ASSERT_MESSAGE("SOR failed to solve linear system", success);



    // compute A x and check that = b
    Vector tmp(x.size());
    tmp = m * x;

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in SOR solver result", SparseLinearSolverUtil::isVectorEqual(tmp, b, 1E-10));
}

void
SparseLinearSolverTest::fs_680_1CGTest() {
    /* FS 680 1: Chemical kinetics problems
     * RCHEM radiation chemistry study -- 1st output time step
     * from set FACSIMILE, from the Harwell-Boeing Collection
     *
     * Note: fs_680_1 is a 680x680 real unsymmetric matrix which
     * is not diagonally dominant.
     */

    // read matrix m
    FS::path filename("\\Develop\\SparseMatrixData\\fs_680_1\\fs_680_1.ar");
    ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());

    SparseMatrix2D const m = sm_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 680ull, m.cols());

    CPPUNIT_ASSERT_MESSAGE("Matrix symmetric", !LinAlg_NS::helper::isSymmetric(m));

    // m is NOT diagonally dominant
    CPPUNIT_ASSERT_MESSAGE("matrix should not be diagonally dominant", !SparseLinearSolverUtil::isStrictlyDiagonallyDominant(m));


    // create solution vector x
    Vector x_ref(680);
    std::iota(std::begin(x_ref), std::end(x_ref), 1);

    // create vector b
    Vector b(680);
    b = m * x_ref;



    bool success;
    Vector x(b.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // CG cannot solve this matrix as it is not symmetric
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::CG(m, b, 1000);
    }

    CPPUNIT_ASSERT_MESSAGE("CG should have failed to solve linear system", !success);
}

void
SparseLinearSolverTest::fs_680_1BiCGTest() {
    /* FS 680 1: Chemical kinetics problems
     * RCHEM radiation chemistry study -- 1st output time step
     * from set FACSIMILE, from the Harwell-Boeing Collection
     *
     * Note: fs_680_1 is a 680x680 real unsymmetric matrix which
     * is not diagonally dominant.
     */

    // read matrix m
    FS::path filename("\\Develop\\SparseMatrixData\\fs_680_1\\fs_680_1.ar");
    ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());

    SparseMatrix2D const m = sm_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 680ull, m.cols());

    CPPUNIT_ASSERT_MESSAGE("Matrix symmetric", !LinAlg_NS::helper::isSymmetric(m));

    // m is NOT diagonally dominant
    CPPUNIT_ASSERT_MESSAGE("matrix should not be diagonally dominant", !SparseLinearSolverUtil::isStrictlyDiagonallyDominant(m));


    // create solution vector x
    Vector x_ref(680);
    std::iota(std::begin(x_ref), std::end(x_ref), 1);

    // create vector b
    Vector b(680);
    b = m * x_ref;



    bool success;
    Vector x(b.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 533 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::BiCG(m, LinAlg_NS::helper::transpose(m), b, 10000);
    }

    CPPUNIT_ASSERT_MESSAGE("BiCG failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in BiCG solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref, 1E-8));
}

void
SparseLinearSolverTest::fs_680_1BiCGSTABTest() {
    /* FS 680 1: Chemical kinetics problems
     * RCHEM radiation chemistry study -- 1st output time step
     * from set FACSIMILE, from the Harwell-Boeing Collection
     *
     * Note: fs_680_1 is a 680x680 real unsymmetric matrix which
     * is not diagonally dominant.
     */

    // read matrix m
    FS::path filename("\\Develop\\SparseMatrixData\\fs_680_1\\fs_680_1.ar");
    ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());

    SparseMatrix2D const m = sm_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 680ull, m.cols());

    CPPUNIT_ASSERT_MESSAGE("Matrix symmetric", !LinAlg_NS::helper::isSymmetric(m));

    // m is NOT diagonally dominant
    CPPUNIT_ASSERT_MESSAGE("matrix should not be diagonally dominant", !SparseLinearSolverUtil::isStrictlyDiagonallyDominant(m));


    // create solution vector x
    Vector x_ref(680);
    std::iota(std::begin(x_ref), std::end(x_ref), 1);

    // create vector b
    Vector b(680);
    b = m * x_ref;



    bool success;
    Vector x(b.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 419 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::BiCGSTAB(m, b, 10000);
    }

    CPPUNIT_ASSERT_MESSAGE("BiCGSTAB failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in BiCGSTAB solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref, 1E-8));
}

void
SparseLinearSolverTest::fs_680_1GMRESTest() {
    /* FS 680 1: Chemical kinetics problems
     * RCHEM radiation chemistry study -- 1st output time step
     * from set FACSIMILE, from the Harwell-Boeing Collection
     *
     * Note: fs_680_1 is a 680x680 real unsymmetric matrix which
     * is not diagonally dominant.
     */

    // read matrix m
    FS::path filename("\\Develop\\SparseMatrixData\\fs_680_1\\fs_680_1.ar");
    ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());

    SparseMatrix2D const m = sm_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 680ull, m.cols());

    CPPUNIT_ASSERT_MESSAGE("Matrix symmetric", !LinAlg_NS::helper::isSymmetric(m));

    // m is NOT diagonally dominant
    CPPUNIT_ASSERT_MESSAGE("matrix should not be diagonally dominant", !SparseLinearSolverUtil::isStrictlyDiagonallyDominant(m));


    // create solution vector x
    Vector x_ref(680);
    std::iota(std::begin(x_ref), std::end(x_ref), 1);

    // create vector b
    Vector b(680);
    b = m * x_ref;



    bool success;
    Vector x(b.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 282 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::GMRES(m, b, 680, 10000);
    }

    CPPUNIT_ASSERT_MESSAGE("GMRES failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in GMRES solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref, 1E-8));
}

void
SparseLinearSolverTest::fs_680_1TFQMRTest() {
    /* FS 680 1: Chemical kinetics problems
     * RCHEM radiation chemistry study -- 1st output time step
     * from set FACSIMILE, from the Harwell-Boeing Collection
     *
     * Note: fs_680_1 is a 680x680 real unsymmetric matrix which
     * is not diagonally dominant.
     */

    // read matrix m
    FS::path filename("\\Develop\\SparseMatrixData\\fs_680_1\\fs_680_1.ar");
    ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());

    SparseMatrix2D const m = sm_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 680ull, m.cols());

    CPPUNIT_ASSERT_MESSAGE("Matrix symmetric", !LinAlg_NS::helper::isSymmetric(m));

    // m is NOT diagonally dominant
    CPPUNIT_ASSERT_MESSAGE("matrix should not be diagonally dominant", !SparseLinearSolverUtil::isStrictlyDiagonallyDominant(m));


    // create solution vector x
    Vector x_ref(680);
    std::iota(std::begin(x_ref), std::end(x_ref), 1);

    // create vector b
    Vector b(680);
    b = m * x_ref;



    bool success;
    Vector x(b.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 517 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::TFQMR(m, b, 10000);
    }

    CPPUNIT_ASSERT_MESSAGE("TFQMR failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in TFQMR solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref, 1E-7));
}

void
SparseLinearSolverTest::spiralMINRESTest() {
    // spiral is symmetric, not pos. def.
    // condition nr: 390841

    // read matrix m
    FS::path filename("\\Develop\\SparseMatrixData\\spiral\\spiral.ar");
    ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());

    SparseMatrix2D const m = sm_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 1434ull, m.cols());

    CPPUNIT_ASSERT_MESSAGE("Matrix not symmetric", LinAlg_NS::helper::isSymmetric(m));

    // m is NOT diagonally dominant
    CPPUNIT_ASSERT_MESSAGE("matrix should not be diagonally dominant", !SparseLinearSolverUtil::isStrictlyDiagonallyDominant(m));


    // create solution vector x
    Vector x_ref(1434);
    std::iota(std::begin(x_ref), std::end(x_ref), 1);

    // create vector b
    Vector b(1434);
    b = m * x_ref;



    bool success;
    Vector x(b.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 1989 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::MINRES(m, b, 10000);
    }

    CPPUNIT_ASSERT_MESSAGE("MINRES failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in MINRES solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref, 2.56 * 1E-8));
}

void
SparseLinearSolverTest::spiralMINRESLanProTest() {
    // spiral is symmetric, not pos. def.
    // condition nr: 390841

    // read matrix m
    FS::path filename("\\Develop\\SparseMatrixData\\spiral\\spiral.ar");
    ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());

    SparseMatrix2D const m = sm_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 1434ull, m.cols());

    CPPUNIT_ASSERT_MESSAGE("Matrix not symmetric", LinAlg_NS::helper::isSymmetric(m));

    // m is NOT diagonally dominant
    CPPUNIT_ASSERT_MESSAGE("matrix should not be diagonally dominant", !SparseLinearSolverUtil::isStrictlyDiagonallyDominant(m));


    // create solution vector x
    Vector x_ref(1434);
    std::iota(std::begin(x_ref), std::end(x_ref), 1);

    // create vector b
    Vector b(1434);
    b = m * x_ref;



    bool success;
    Vector x(b.size());
    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // needs 634 iterations vs 1989 without partial reorthogonalization
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::MINRESLanPro(m, b, 10000);
    }

    CPPUNIT_ASSERT_MESSAGE("MINRESLanPRO failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in MINRESLanPRO solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref, 9.8 * 1E-7));
}
