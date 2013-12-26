#include "pch.h"

#include "LinAlgTest.h"

#include "LinAlg/Matrix2D.h"
#include "LinAlg/SparseMatrix2D.h"


using namespace LinAlg_NS;
using namespace EntityReader_NS;

namespace FS = boost::filesystem;


void
LinAlgTest::setUp() {}

void
LinAlgTest::tearDown() {}


void
LinAlgTest::transposeVersteegMalalasekeraTest() {
    // read matrix m
    FS::path filename("\\Develop\\SparseMatrixData\\Versteeg_Malalasekera\\Versteeg_Malalasekera.ar");
    ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("File not found", sm_reader);
    CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());

    SparseMatrix2D const m = sm_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 18ull, m.cols());


    // create transpose
    SparseMatrix2D transpose = LinAlg_NS::helper::transpose(m);

    for (SparseMatrix2D::size_type row = 0; row < m.rows(); ++row) {
        for (SparseMatrix2D::size_type col = 0; col < m.cols(); ++col) {
            double value = m(row, col);
            double other_value = m(col, row);

            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Matrix value mismatch", value, other_value, 1E-10);
        }
    }
}

void
LinAlgTest::transpose_fs_680_1_Test() {
    // read matrix m
    FS::path filename("\\Develop\\SparseMatrixData\\fs_680_1\\fs_680_1.ar");
    ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("File not found", sm_reader);
    CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());

    SparseMatrix2D const m = sm_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 680ull, m.cols());


    // create transpose
    SparseMatrix2D const transpose = LinAlg_NS::helper::transpose(m);

    bool is_symmetric = true;

    for (SparseMatrix2D::size_type row = 0; row < m.rows(); ++row) {
        for (SparseMatrix2D::size_type col = 0; col < m.cols(); ++col) {
            double value = m(row, col);
            double other_value = transpose(col, row);

           CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Matrix value mismatch", value, other_value, 1E-10);
        }
    }
}
