#include "pch.h"

#include "UHMatrixTest.h"

#include "LinAlg/UHMatrix.h"


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
//     tmp = m(0, 3);
//     tmp = m(3, 3);

//     CPPUNIT_ASSERT_THROW_MESSAGE("Invalid access", m(1, 0), std::out_of_range);
//     CPPUNIT_ASSERT_THROW_MESSAGE("Invalid access", m(3, 2), std::out_of_range);


    m.print();
}
