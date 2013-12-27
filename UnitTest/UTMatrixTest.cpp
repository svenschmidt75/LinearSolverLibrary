#include "pch.h"

#include "UTMatrixTest.h"


using namespace LinAlg_NS;
using namespace EntityReader_NS;

namespace FS = boost::filesystem;


void
UTMatrixTest::setUp() {}

void
UTMatrixTest::tearDown() {}

namespace {

    UTMatrix
    fill(UTMatrix::size_type dim) {
        UTMatrix m(dim);

        double value = 1.0;

        for (UTMatrix::size_type col = 0; col < dim; ++col) {
            for (UTMatrix::size_type row = 0; row <= col; ++row) {
                m(row, col) = value++;
            }
        }

        return m;
    }

}

void
UTMatrixTest::indexTest() {
    UTMatrix m = fill(4);

    // tests index limits (making sure no exception is thrown)
    double tmp = m(0, 0);
    tmp = m(0, 3);
    tmp = m(3, 3);

    CPPUNIT_ASSERT_THROW_MESSAGE("Invalid access", m(1, 0), std::out_of_range);
    CPPUNIT_ASSERT_THROW_MESSAGE("Invalid access", m(3, 2), std::out_of_range);

//    m.print();
}
