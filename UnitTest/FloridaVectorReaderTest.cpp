#include "pch.h"

#include "FloridaVectorReaderTest.h"


using namespace EntityReader_NS;
using namespace LinAlg_NS;

namespace FS = boost::filesystem;


void
FloridaVectorReaderTest::setUp() {}

void
FloridaVectorReaderTest::tearDown() {}

void
FloridaVectorReaderTest::readSTS4098Test() {
    /* We do not directly test the reading of the mtx files as
     * this takes way too long for a unit test.
     * Instead, we test the deserialized binary representation
     * that the project "FloridaVectorReader" generates.
     * That project uses the builder.
     */
    FS::path filename("\\Develop\\SparseMatrixData\\sts4098\\sts4098_b.ar");

    IVectorReader::Ptr sm_reader = VectorReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("File not found", sm_reader);
    CPPUNIT_ASSERT_MESSAGE("error reading vector data", sm_reader->read());

    Vector const m = sm_reader->get();

    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in vector size", 4098ull, m.size());

    // check the extreme values
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("vector data error", -5.589998170733452, m(1 - 1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("vector data error", -.231875017285347, m(4096 - 1), 1E-10);

    // check some values
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("vector data error", -10.34446096420288, m(1162 - 1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("vector data error", .2781918793916702, m(1970 - 1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("vector data error", -.5157000347971916, m(2436 - 1), 1E-10);
}

void
FloridaVectorReaderTest::readDeter7Test() {
    /* We do not directly test the reading of the mtx files as
     * this takes way too long for a unit test.
     * Instead, we test the deserialized binary representation
     * that the project "FloridaVectorReader" generates.
     * That project uses the builder.
     */
    FS::path filename("\\Develop\\SparseMatrixData\\deter7\\deter7_b.ar");

    IVectorReader::Ptr sm_reader = VectorReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("File not found", sm_reader);
    CPPUNIT_ASSERT_MESSAGE("error reading vector data", sm_reader->read());

    Vector const m = sm_reader->get();

    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in vector size", 6375ull, m.size());

    // check the extreme values
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("vector data error", 0, m(1 - 1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("vector data error", 0, m(6375 - 1), 1E-10);

    // check some values
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("vector data error", .2, m(2 - 1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("vector data error", .3, m(3 - 1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("vector data error", 0, m(5 - 1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("vector data error", .3, m(7 - 1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("vector data error", .2, m(13 - 1), 1E-10);
}

void
FloridaVectorReaderTest::readLF10Test() {
    /* We do not directly test the reading of the mtx files as
     * this takes way too long for a unit test.
     * Instead, we test the deserialized binary representation
     * that the project "FloridaVectorReader" generates.
     * That project uses the builder.
     */
    FS::path filename("\\Develop\\SparseMatrixData\\LF10\\LF10_b.ar");

    IVectorReader::Ptr sm_reader = VectorReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("File not found", sm_reader);
    CPPUNIT_ASSERT_MESSAGE("error reading vector data", sm_reader->read());

    Vector const m = sm_reader->get();

    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in vector size", 18ull, m.size());

    // check the extreme values
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("vector data error", 0, m(1 - 1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("vector data error", 0, m(18 - 1), 1E-10);

    // check some values
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("vector data error", 1.0, m(8 - 1), 1E-10);
}
