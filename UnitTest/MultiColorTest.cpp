#include "pch.h"

#include "MultiColorTest.h"

#include "LinAlg/SparseMatrix2D.h"
#include "LinAlg/operators.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


void
MultiColorTest::setUp() {}

void
MultiColorTest::tearDown() {}

namespace {
    bool noElementOnLowerLeft(SparseMatrix2D const & m, int row) {
        return false;
    }

}

void
MultiColorTest::simpleTest() {
    /* From
     * "Solving the Poisson Problem in Parallel with S.O.R."
     * by Landon Boyd
     */
    SparseMatrix2D m(9);

    m(0, 0) = -4;
    m(0, 1) = 1;
    m(0, 3) = 1;

    m(1, 0) = 1;
    m(1, 1) = -4;
    m(1, 2) = 1;
    m(1, 4) = 1;

    m(2, 1) = 1;
    m(2, 2) = -4;
    m(2, 5) = 1;

    m(3, 0) = 1;
    m(3, 3) = -4;
    m(3, 4) = 1;
    m(3, 6) = 1;

    m(4, 1) = 1;
    m(4, 3) = 1;
    m(4, 4) = -4;
    m(4, 5) = 1;
    m(4, 7) = 1;

    m(5, 2) = 1;
    m(5, 4) = 1;
    m(5, 5) = -4;
    m(5, 8) = 1;

    m(6, 3) = 1;
    m(6, 6) = -4;
    m(6, 7) = 1;

    m(7, 4) = 1;
    m(7, 6) = 1;
    m(7, 7) = -4;
    m(7, 8) = 1;

    m(8, 5) = 1;
    m(8, 7) = 1;
    m(8, 8) = -4;

    m.finalize();

    m.print();


    // find independent sets and return new matrix
    SparseMatrix2D new_m = SparseLinearSolverUtil::(m);

    // the first 5 rows should have no elements on the lower-left part
    CPPUNIT_ASSERT_EQUAL_MESSAGE("failure in multicoloring", true, noElementOnLowerLeft(m, 0));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("failure in multicoloring", true, noElementOnLowerLeft(m, 1));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("failure in multicoloring", true, noElementOnLowerLeft(m, 2));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("failure in multicoloring", true, noElementOnLowerLeft(m, 3));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("failure in multicoloring", true, noElementOnLowerLeft(m, 4));
}
