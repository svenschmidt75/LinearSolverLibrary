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

void
SparseLinearSolverTest::VersteegMalalasekeraSORTest() {
    // read matrix m
    FS::path filename("\\Develop\\SparseMatrixData\\Versteeg_Malalasekera\\Versteeg_Malalasekera.ar");
    ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("File not found", sm_reader);
    CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());

    SparseMatrix2D const m = sm_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 18ull, m.cols());


    // check m is diagonally dominant
    CPPUNIT_ASSERT_MESSAGE("matrix not diagonally dominant", SparseLinearSolverUtil::isStrictlyDiagonallyDominant(m));



    // read rhs vector b
    filename = "\\Develop\\SparseMatrixData\\Versteeg_Malalasekera\\Versteeg_Malalasekera_b.ar";
    IVectorReader::Ptr b_reader = VectorReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading vector data", b_reader->read());

    Vector const b = b_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in vector size", 18ull, b.size());



    // read solution vector x
    filename = "\\Develop\\SparseMatrixData\\Versteeg_Malalasekera\\Versteeg_Malalasekera_x.ar";
    IVectorReader::Ptr x_reader = VectorReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading vector data", x_reader->read());

    Vector const x_ref = x_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in vector size", 18ull, x_ref.size());

    bool success;
    Vector x(x_ref.size());
    int iterations;
    std::tie(success, x, iterations) = SparseLinearSolverLibrary::sparseSOR(m, b, 1.1, 10000);

    // needs 33 iterations

    CPPUNIT_ASSERT_MESSAGE("SOR failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in SOR solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref, 1E-10));




    // compute A x and check that = b
    Vector tmp(x.size());
    tmp = m * x;

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in SOR solver result", SparseLinearSolverUtil::isVectorEqual(tmp, b, 1E-10));
}

void
SparseLinearSolverTest::VersteegMalalasekeraSORMultiColorTest() {
    // read matrix m
    FS::path filename("\\Develop\\SparseMatrixData\\Versteeg_Malalasekera\\Versteeg_Malalasekera.ar");
    ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("File not found", sm_reader);
    CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());

    SparseMatrix2D const m = sm_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 18ull, m.cols());


    // check m is diagonally dominant
    CPPUNIT_ASSERT_MESSAGE("matrix not diagonally dominant", SparseLinearSolverUtil::isStrictlyDiagonallyDominant(m));



    // read rhs vector b
    filename = "\\Develop\\SparseMatrixData\\Versteeg_Malalasekera\\Versteeg_Malalasekera_b.ar";
    IVectorReader::Ptr b_reader = VectorReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading vector data", b_reader->read());

    Vector const b = b_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in vector size", 18ull, b.size());



    // read solution vector x
    filename = "\\Develop\\SparseMatrixData\\Versteeg_Malalasekera\\Versteeg_Malalasekera_x.ar";
    IVectorReader::Ptr x_reader = VectorReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading vector data", x_reader->read());

    Vector const x_ref = x_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in vector size", 18ull, x_ref.size());

    // compute multi-color decomposition
    MatrixDecomposition m_decomp = SparseLinearSolverUtil::multicolorDecomposition(m);

//    m.print();

    bool success;
    Vector x(x_ref.size());
    int iterations;
    std::tie(success, x, iterations) = SparseLinearSolverLibrary::sparseSORMultiColor(m, b, m_decomp, 1.1, 10000);

    // needs 32 iterations

    CPPUNIT_ASSERT_MESSAGE("SOR failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in SOR solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref, 1E-10));




    // compute A x and check that = b
    Vector tmp(x.size());
    tmp = m * x;

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in SOR solver result", SparseLinearSolverUtil::isVectorEqual(tmp, b, 1E-10));
}

void
SparseLinearSolverTest::VersteegMalalasekeraCGTest() {
    // read matrix m
    FS::path filename("\\Develop\\SparseMatrixData\\Versteeg_Malalasekera\\Versteeg_Malalasekera.ar");
    ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("File not found", sm_reader);
    CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());

    SparseMatrix2D const m = sm_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 18ull, m.cols());


    // check m is diagonally dominant
    CPPUNIT_ASSERT_MESSAGE("matrix not diagonally dominant", SparseLinearSolverUtil::isStrictlyDiagonallyDominant(m));



    // read rhs vector b
    filename = "\\Develop\\SparseMatrixData\\Versteeg_Malalasekera\\Versteeg_Malalasekera_b.ar";
    IVectorReader::Ptr b_reader = VectorReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading vector data", b_reader->read());

    Vector const b = b_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in vector size", 18ull, b.size());



    // read solution vector x
    filename = "\\Develop\\SparseMatrixData\\Versteeg_Malalasekera\\Versteeg_Malalasekera_x.ar";
    IVectorReader::Ptr x_reader = VectorReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading vector data", x_reader->read());

    Vector const x_ref = x_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in vector size", 18ull, x_ref.size());

//   m.print();

    bool success;
    Vector x(x_ref.size());
    int iterations;
    double tol;
    std::tie(success, x, iterations, tol) = ConjugateGradientMethods::CG(m, b, 10000);

    // needs 4 iterations

    CPPUNIT_ASSERT_MESSAGE("CG failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in CG solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref, 1E-10));




    // compute A x and check that = b
    Vector tmp(x.size());
    tmp = m * x;

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in CG solver result", SparseLinearSolverUtil::isVectorEqual(tmp, b, 1E-10));
}

void
SparseLinearSolverTest::VersteegMalalasekeraBiCGTest() {
    // read matrix m
    FS::path filename("\\Develop\\SparseMatrixData\\Versteeg_Malalasekera\\Versteeg_Malalasekera.ar");
    ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("File not found", sm_reader);
    CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());

    SparseMatrix2D const m = sm_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 18ull, m.cols());


    // check m is diagonally dominant
    CPPUNIT_ASSERT_MESSAGE("matrix not diagonally dominant", SparseLinearSolverUtil::isStrictlyDiagonallyDominant(m));



    // read rhs vector b
    filename = "\\Develop\\SparseMatrixData\\Versteeg_Malalasekera\\Versteeg_Malalasekera_b.ar";
    IVectorReader::Ptr b_reader = VectorReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading vector data", b_reader->read());

    Vector const b = b_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in vector size", 18ull, b.size());



    // read solution vector x
    filename = "\\Develop\\SparseMatrixData\\Versteeg_Malalasekera\\Versteeg_Malalasekera_x.ar";
    IVectorReader::Ptr x_reader = VectorReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading vector data", x_reader->read());

    Vector const x_ref = x_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in vector size", 18ull, x_ref.size());

    //   m.print();

    bool success;
    Vector x(x_ref.size());
    int iterations;
    double tol;

    // - calling transpose here is unnecessary, as m is symmetric
    // - this is why BiCG is no better than CG
    std::tie(success, x, iterations, tol) = ConjugateGradientMethods::BiCG(m, LinAlg_NS::helper::transpose(m), b, 10000);

    // needs 4 iterations

    CPPUNIT_ASSERT_MESSAGE("BiCG failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in BiCG solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref, 1E-10));




    // compute A x and check that = b
    Vector tmp(x.size());
    tmp = m * x;

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in BiCG solver result", SparseLinearSolverUtil::isVectorEqual(tmp, b, 1E-10));
}

void
SparseLinearSolverTest::VersteegMalalasekeraBiCGSTABTest() {
    // read matrix m
    FS::path filename("\\Develop\\SparseMatrixData\\Versteeg_Malalasekera\\Versteeg_Malalasekera.ar");
    ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("File not found", sm_reader);
    CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());

    SparseMatrix2D const m = sm_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 18ull, m.cols());


    // check m is diagonally dominant
    CPPUNIT_ASSERT_MESSAGE("matrix not diagonally dominant", SparseLinearSolverUtil::isStrictlyDiagonallyDominant(m));



    // read rhs vector b
    filename = "\\Develop\\SparseMatrixData\\Versteeg_Malalasekera\\Versteeg_Malalasekera_b.ar";
    IVectorReader::Ptr b_reader = VectorReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading vector data", b_reader->read());

    Vector const b = b_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in vector size", 18ull, b.size());



    // read solution vector x
    filename = "\\Develop\\SparseMatrixData\\Versteeg_Malalasekera\\Versteeg_Malalasekera_x.ar";
    IVectorReader::Ptr x_reader = VectorReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading vector data", x_reader->read());

    Vector const x_ref = x_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in vector size", 18ull, x_ref.size());

    //   m.print();

    bool success;
    Vector x(x_ref.size());
    int iterations;
    double tol;

    std::tie(success, x, iterations, tol) = ConjugateGradientMethods::BiCGSTAB(m, b, 10000);

    // needs 4 iterations

    CPPUNIT_ASSERT_MESSAGE("BiCGSTAB failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in BiCGSTAB solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref, 1E-10));




    // compute A x and check that = b
    Vector tmp(x.size());
    tmp = m * x;

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in BiCGSTAB solver result", SparseLinearSolverUtil::isVectorEqual(tmp, b, 1E-10));
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
SparseLinearSolverTest::VersteegMalalasekeraGMRESTest() {
    // read matrix m
    FS::path filename("\\Develop\\SparseMatrixData\\Versteeg_Malalasekera\\Versteeg_Malalasekera.ar");
    ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("File not found", sm_reader);
    CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());

    SparseMatrix2D const m = sm_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 18ull, m.cols());


    // check m is diagonally dominant
    CPPUNIT_ASSERT_MESSAGE("matrix not diagonally dominant", SparseLinearSolverUtil::isStrictlyDiagonallyDominant(m));



    // read rhs vector b
    filename = "\\Develop\\SparseMatrixData\\Versteeg_Malalasekera\\Versteeg_Malalasekera_b.ar";
    IVectorReader::Ptr b_reader = VectorReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading vector data", b_reader->read());

    Vector const b = b_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in vector size", 18ull, b.size());



    // read solution vector x
    filename = "\\Develop\\SparseMatrixData\\Versteeg_Malalasekera\\Versteeg_Malalasekera_x.ar";
    IVectorReader::Ptr x_reader = VectorReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading vector data", x_reader->read());

    Vector const x_ref = x_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in vector size", 18ull, x_ref.size());

    //   m.print();

    bool success;
    Vector x(x_ref.size());
    int iterations;
    double tol;

    {
        HighResTimer t;

        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::GMRES(m, b, 6, 10000);
    }

    // needs 5 iterations

    CPPUNIT_ASSERT_MESSAGE("GMRES failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in GMRES solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref, 1E-10));




    // compute A x and check that = b
    Vector tmp(x.size());
    tmp = m * x;

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in GMRES solver result", SparseLinearSolverUtil::isVectorEqual(tmp, b, 1E-10));
}

void
SparseLinearSolverTest::bcsstk05SORTest() {
    // BCSSTK05 is symmetric pos. def., but not diagonally dominant

    // read matrix m
    FS::path filename("\\Develop\\SparseMatrixData\\bcsstk05\\bcsstk05.ar");
    ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());

    SparseMatrix2D const m = sm_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 153ull, m.cols());

    CPPUNIT_ASSERT_MESSAGE("Matrix not symmetric", LinAlg_NS::helper::isSymmsteric(m));

    // m is NOT diagonally dominant
    CPPUNIT_ASSERT_MESSAGE("matrix should not be diagonally dominant", !SparseLinearSolverUtil::isStrictlyDiagonallyDominant(m));


    // create solution vector x
    Vector x_ref(153);
    std::iota(std::begin(x_ref), std::end(x_ref), 1);

    // create vector b
    Vector b(153);
    b = m * x_ref;



    bool success;
    Vector x(b.size());
    int iterations;
    std::tie(success, x, iterations) = SparseLinearSolverLibrary::sparseSOR(m, b, 1.1, 20000);

    // needs 17687 iterations

    CPPUNIT_ASSERT_MESSAGE("SOR failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in SOR solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref, 1E-10));
}

void
SparseLinearSolverTest::bcsstk05CGTest() {
    // BCSSTK05 is symmetric pos. def., but not diagonally dominant

    // read matrix m
    FS::path filename("\\Develop\\SparseMatrixData\\bcsstk05\\bcsstk05.ar");
    ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());

    SparseMatrix2D const m = sm_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 153ull, m.cols());

    CPPUNIT_ASSERT_MESSAGE("Matrix not symmetric", LinAlg_NS::helper::isSymmsteric(m));

    // m is NOT diagonally dominant
    CPPUNIT_ASSERT_MESSAGE("matrix should not be diagonally dominant", !SparseLinearSolverUtil::isStrictlyDiagonallyDominant(m));


    // create solution vector x
    Vector x_ref(153);
    std::iota(std::begin(x_ref), std::end(x_ref), 1);

    // create vector b
    Vector b(153);
    b = m * x_ref;



    bool success;
    Vector x(b.size());
    int iterations;
    double tol;

    {
        HighResTimer t;

        // needs 321 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::CG(m, b, 10000);
    }

    CPPUNIT_ASSERT_MESSAGE("CG failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in CG solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref, 1E-10));
}

void
SparseLinearSolverTest::bcsstk05BiCGTest() {
    // BCSSTK05 is symmetric pos. def., but not diagonally dominant

    // read matrix m
    FS::path filename("\\Develop\\SparseMatrixData\\bcsstk05\\bcsstk05.ar");
    ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());

    SparseMatrix2D const m = sm_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 153ull, m.cols());

    CPPUNIT_ASSERT_MESSAGE("Matrix not symmetric", LinAlg_NS::helper::isSymmsteric(m));

    // m is NOT diagonally dominant
    CPPUNIT_ASSERT_MESSAGE("matrix should not be diagonally dominant", !SparseLinearSolverUtil::isStrictlyDiagonallyDominant(m));


    // create solution vector x
    Vector x_ref(153);
    std::iota(std::begin(x_ref), std::end(x_ref), 1);

    // create vector b
    Vector b(153);
    b = m * x_ref;



    bool success;
    Vector x(b.size());
    int iterations;
    double tol;

    {
        HighResTimer t;

        // needs 321 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::BiCG(m, LinAlg_NS::helper::transpose(m), b, 10000);
    }

    CPPUNIT_ASSERT_MESSAGE("BiCG failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in BiCG solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref, 1E-10));
}

void
SparseLinearSolverTest::bcsstk05BiCGSTABTest() {
    // BCSSTK05 is symmetric pos. def., but not diagonally dominant

    // read matrix m
    FS::path filename("\\Develop\\SparseMatrixData\\bcsstk05\\bcsstk05.ar");
    ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());

    SparseMatrix2D const m = sm_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 153ull, m.cols());

    CPPUNIT_ASSERT_MESSAGE("Matrix not symmetric", LinAlg_NS::helper::isSymmsteric(m));

    // m is NOT diagonally dominant
    CPPUNIT_ASSERT_MESSAGE("matrix should not be diagonally dominant", !SparseLinearSolverUtil::isStrictlyDiagonallyDominant(m));


    // create solution vector x
    Vector x_ref(153);
    std::iota(std::begin(x_ref), std::end(x_ref), 1);

    // create vector b
    Vector b(153);
    b = m * x_ref;



    bool success;
    Vector x(b.size());
    int iterations;
    double tol;

    {
        HighResTimer t;

        // needs 355 iterations
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::BiCGSTAB(m, b, 10000);
    }

    CPPUNIT_ASSERT_MESSAGE("BiCGSTAB failed to solve linear system", success);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in BiCGSTAB solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref, 1E-10));
}

void
SparseLinearSolverTest::sts4098SORTest() {
    // read matrix m
    FS::path filename("\\Develop\\SparseMatrixData\\sts4098\\sts4098.ar");
    ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());

    SparseMatrix2D const m = sm_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 4098ull, m.cols());

    CPPUNIT_ASSERT_MESSAGE("Matrix not symmetric", LinAlg_NS::helper::isSymmsteric(m));

    // m is NOT diagonally dominant
    CPPUNIT_ASSERT_MESSAGE("matrix should not be diagonally dominant", !SparseLinearSolverUtil::isStrictlyDiagonallyDominant(m));




    // read rhs vector b
    filename = "\\Develop\\SparseMatrixData\\sts4098\\sts4098_b.ar";
    IVectorReader::Ptr b_reader = VectorReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading vector data", b_reader->read());

    Vector const b = b_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in vector size", 4098ull, b.size());



    bool success;
    Vector x(b.size());
    int iterations;

    {
        HighResTimer t;

        // needs 10986 iterations
        std::tie(success, x, iterations) = SparseLinearSolverLibrary::sparseSOR(m, b, 1.1, 11000);
    }

    CPPUNIT_ASSERT_MESSAGE("SOR failed to solve linear system", success);


    // compute A x and check that = b
    Vector tmp(x.size());
    tmp = m * x;

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in SOR solver result", SparseLinearSolverUtil::isVectorEqual(tmp, b, 1E-10));
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

    CPPUNIT_ASSERT_MESSAGE("Matrix symmetric", !LinAlg_NS::helper::isSymmsteric(m));

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
    int iterations;
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

    CPPUNIT_ASSERT_MESSAGE("Matrix symmetric", !LinAlg_NS::helper::isSymmsteric(m));

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
    int iterations;
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

    CPPUNIT_ASSERT_MESSAGE("Matrix symmetric", !LinAlg_NS::helper::isSymmsteric(m));

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
    int iterations;
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
