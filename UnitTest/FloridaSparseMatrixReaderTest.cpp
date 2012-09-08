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
    SparseMatrix2D m;

    ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());

    m = sm_reader->get();


    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 4098ull, m.cols());

//     CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", 0.1, result(0), 1E-10);
//     CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", 1.0, result(9), 1E-10);
// 
// 
//     ar & BOOST_SERIALIZATION_NVP(m.ncols_);
//     ar & BOOST_SERIALIZATION_NVP(m.data_);
//     ar & BOOST_SERIALIZATION_NVP(m.finalized_);
//     ar & BOOST_SERIALIZATION_NVP(m.elements_);
//     ar & BOOST_SERIALIZATION_NVP(m.columns_);
//     ar & BOOST_SERIALIZATION_NVP(m.nelements_);



    // assert #elements
    // dim
    // a couple of non-zero entries
}
