#include "FloridaSparseMatrixReaderTest.h"

#include "FloridaSparseMatrixReaderLib/FloridaSparseMatrixReader.h"
#include "FloridaSparseMatrixReaderLib/SparseMatrixBuilder.h"
#include "FloridaSparseMatrixReaderLib/ISparseMatrixReader.h"
#include "FloridaSparseMatrixReaderLib/SparseMatrixReaderCreator.h"

#include "LinAlg/SparseMatrix2D.h"

#include <boost/filesystem.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/nvp.hpp>

#include <string>


using namespace EntityReader_NS;
using namespace LinAlg_NS;

namespace FS = boost::filesystem;


void
FloridaSparseMatrixReaderTest::setUp() {}

void
FloridaSparseMatrixReaderTest::tearDown() {}

void
FloridaSparseMatrixReaderTest::readTest() {
    /* We do not directly test the reading of the mtx files as
     * this takes way too long for a unit test.
     * Instead, we test the deserialized binary representation
     * that the project "FloridaSparseMatrixReader" generates.
     * That project uses the builder.
     */
    FS::path filename("\\Develop\\SparseMatrixData\\sts4098\\sts4098.ar");

    ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());

    SparseMatrix2D const m = sm_reader->get();

    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 4098ull, m.cols());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("sparse matrix not finalized after read", true, m.finalized_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("sparse matrix internal data error", 38227ull, m.elements_.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("sparse matrix internal data error", 4099ull, m.nelements_.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("sparse matrix internal data error", 38227ull, m.columns_.size());

    // check some values
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("sparse matrix data error", 409897.3044519858, m(1 - 1, 1 - 1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("sparse matrix data error", -32933.34682143788, m(836 - 1, 1 - 1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("sparse matrix data error", 13.84223585895381, m(1605 - 1, 106 - 1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("sparse matrix data error", -9.65525670802707, m(1695 - 1, 439 - 1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("sparse matrix data error", -14.09773638247952, m(3898 - 1, 1274 - 1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("sparse matrix data error", 17.32241539867855, m(2543 - 1, 2541 - 1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("sparse matrix data error", 6.983992679022847, m(4098 - 1, 4098 - 1), 1E-10);
}
