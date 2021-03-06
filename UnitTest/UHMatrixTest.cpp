#include "pch.h"

#include "UHMatrixTest.h"


using namespace LinAlg_NS;
using namespace EntityReader_NS;

namespace FS = boost::filesystem;


void
UHMatrixTest::setUp() {}

void
UHMatrixTest::tearDown() {}

namespace {

    UHMatrix
    fill(UHMatrix::size_type dim) {
        UHMatrix m(dim);

        double value = 1.0;

        for (UHMatrix::size_type col = 0; col < dim; ++col) {
            for (UHMatrix::size_type row = 0; row <= std::min(col + 1, dim - 1); ++row) {
                m(row, col) = value++;
            }
        }

        return m;
    }

}

void
UHMatrixTest::indexTest() {
    UHMatrix m = fill(4);

    // tests index limits (making sure no exception is thrown)
    double tmp = m(0, 0);
    tmp = m(0, 0);
    tmp = m(1, 0);
    CPPUNIT_ASSERT_THROW_MESSAGE("Invalid access axpected", m(2, 0), std::out_of_range);

    tmp = m(1, 1);
    tmp = m(2, 1);
    CPPUNIT_ASSERT_THROW_MESSAGE("Invalid access axpected", m(3, 1), std::out_of_range);

    tmp = m(3, 2);
    tmp = m(3, 3);

//     m.print();
}
