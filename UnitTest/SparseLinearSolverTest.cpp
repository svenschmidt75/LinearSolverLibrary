#include "pch.h"

#include "SparseLinearSolverTest.h"

#include "LinAlg/Vector.h"
#include "LinAlg/SparseMatrix2D.h"
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
    CPPUNIT_ASSERT_MESSAGE("matrix not diagonally dominant", SparseLinearSolverUtil::isDiagonallyDominant(m));



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
    std::tie(success, x, iterations) = SparseLinearSolver::sparseSOR(m, b, 1.1, 10000);

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
    CPPUNIT_ASSERT_MESSAGE("matrix not diagonally dominant", SparseLinearSolverUtil::isDiagonallyDominant(m));



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
    std::tie(success, x, iterations) = SparseLinearSolver::sparseSORMultiColor(m, b, m_decomp, 1.1, 10000);

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
    CPPUNIT_ASSERT_MESSAGE("matrix not diagonally dominant", SparseLinearSolverUtil::isDiagonallyDominant(m));



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

//    m.print();

    bool success;
    Vector x {x_ref.size()};
    int iterations;
    std::tie(success, x, iterations) = ConjugateGradientMethods::conjugateGradient(m, std::function<void ()>(), b, 10000);

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in CG solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref, 1E-10));




    // compute A x and check that = b
    Vector tmp(x.size());
    tmp = m * x;

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in SOR solver result", SparseLinearSolverUtil::isVectorEqual(tmp, b, 1E-10));
}

void
SparseLinearSolverTest::sts4098SORTest() {
    // read matrix m
    FS::path filename("\\Develop\\SparseMatrixData\\sts4098\\sts4098.ar");
    ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());

    SparseMatrix2D const m = sm_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 4098ull, m.cols());


    // m is NOT diagonally dominant
    CPPUNIT_ASSERT_MESSAGE("matrix should not be diagonally dominant", !SparseLinearSolverUtil::isDiagonallyDominant(m));




    // read rhs vector b
    filename = "\\Develop\\SparseMatrixData\\sts4098\\sts4098_b.ar";
    IVectorReader::Ptr b_reader = VectorReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading vector data", b_reader->read());

    Vector const b = b_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in vector size", 4098ull, b.size());



    bool success;
    Vector x(b.size());
    int iterations;
    std::tie(success, x, iterations) = SparseLinearSolver::sparseSOR(m, b, 1.1, 10000);



    // compute A x and check that = b
    Vector tmp(x.size());
    tmp = m * x;

    // compare vectors
    CPPUNIT_ASSERT_MESSAGE("mismatch in SOR solver result", SparseLinearSolverUtil::isVectorEqual(tmp, b, 1E-10));
}
