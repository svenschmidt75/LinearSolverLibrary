#include "pch.h"

#include "FloridaSparseMatrixReaderTest.h"


using namespace EntityReader_NS;
using namespace LinAlg_NS;

namespace FS = boost::filesystem;


void
FloridaSparseMatrixReaderTest::setUp() {}

void
FloridaSparseMatrixReaderTest::tearDown() {}

void
FloridaSparseMatrixReaderTest::symmetryStrategyTest() {
    SparseMatrix2D m(3);

    // initialize the matrix s.t. it is NOT symmetric
    m(0, 0) = 1;
    m(0, 1) = 2;
    m(0, 2) = 3;
    m(1, 0) = 4;
    m(1, 1) = 5;
    m(1, 2) = 6;
    m(2, 0) = 7;
    m(2, 1) = 8;
    m(2, 2) = 9;

    ISymmetryStrategy::UniquePtr symmetry_strategy = SymmetryStrategyFactory::create("symmetric", m);

    symmetry_strategy->insert(2, 1, 2);

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("sparse matrix data error", m(1, 2), m(2, 1), 1E-10);
}

void
FloridaSparseMatrixReaderTest::unknownSymmetryStrategyTest() {
    SparseMatrix2D m(3);

    ISymmetryStrategy::UniquePtr symmetry_strategy = SymmetryStrategyFactory::create("garbage", m);
}

namespace {

    class HighResTimer {
    public:
        HighResTimer() : start_(boost::chrono::high_resolution_clock::now()) {}
        ~HighResTimer() {
            auto end = boost::chrono::high_resolution_clock::now();
            auto frac = (double)boost::chrono::high_resolution_clock::period::num / boost::chrono::high_resolution_clock::period::den;
            auto d = (end - start_).count() * frac;
            std::cout << std::endl << "Duration: " << d << std::endl;
        }

    private:
        boost::chrono::steady_clock::time_point start_;
    };

}

void
FloridaSparseMatrixReaderTest::readSTS4098Test() {
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

    {
        HighResTimer t;
        CPPUNIT_ASSERT_MESSAGE("Matrix not symmetric", LinAlg_NS::helper::isSymmetric(m));
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 4098ull, m.cols());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("sparse matrix not finalized after read", true, m.finalized_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("sparse matrix internal data error", 72356ull, m.elements_.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("sparse matrix internal data error", 4099ull, m.columns_offset_.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("sparse matrix internal data error", 72356ull, m.columns_.size());

    // check some values
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("sparse matrix data error", 409897.3044519858, m(1 - 1, 1 - 1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("sparse matrix data error", -32933.34682143788, m(836 - 1, 1 - 1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("sparse matrix data error", 13.84223585895381, m(1605 - 1, 106 - 1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("sparse matrix data error", -9.65525670802707, m(1695 - 1, 439 - 1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("sparse matrix data error", -14.09773638247952, m(3898 - 1, 1274 - 1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("sparse matrix data error", 17.32241539867855, m(2543 - 1, 2541 - 1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("sparse matrix data error", 6.983992679022847, m(4098 - 1, 4098 - 1), 1E-10);
}

void
FloridaSparseMatrixReaderTest::readFS6801Test() {
    /* We do not directly test the reading of the mtx files as
     * this takes way too long for a unit test.
     * Instead, we test the deserialized binary representation
     * that the project "FloridaSparseMatrixReader" generates.
     * That project uses the builder.
     */
    FS::path filename("\\Develop\\SparseMatrixData\\fs_680_1\\fs_680_1.ar");

    ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());

    SparseMatrix2D const m = sm_reader->get();

    CPPUNIT_ASSERT_MESSAGE("Matrix should not be symmetric", !LinAlg_NS::helper::isSymmetric(m));

    /* Note: The number of non-zero entries is NOT 2646 as this matrix
     * contains explicitly stored 0, which are skipped here.
     */
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 680ull, m.cols());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("sparse matrix not finalized after read", true, m.finalized_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("sparse matrix internal data error", 2184ull, m.elements_.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("sparse matrix internal data error", 681ull, m.columns_offset_.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("sparse matrix internal data error", 2184ull, m.columns_.size());

    // check some values
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("sparse matrix data error", -2828956861.694, m(81 - 1, 1 - 1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("sparse matrix data error", -41393849.94464, m(30 - 1, 31 - 1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("sparse matrix data error", -732611814.3223, m(104 - 1, 105 - 1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("sparse matrix data error", -68982344.42303, m(189 - 1, 190 - 1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("sparse matrix data error", 7711454570.932, m(680 - 1, 680 - 1), 1E-10);
}
