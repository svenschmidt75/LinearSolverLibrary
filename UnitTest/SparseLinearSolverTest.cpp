#include "pch.h"

#include "SparseLinearSolverTest.h"

#include "LinAlg/Vector.h"
#include "LinAlg/SparseMatrix2D.h"


using namespace EntityReader_NS;
using namespace LinAlg_NS;

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
    CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());

    SparseMatrix2D const m = sm_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 18ull, m.cols());


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

    Vector const x = x_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in vector size", 18ull, x.size());

}

