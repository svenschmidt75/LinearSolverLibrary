#include "pch.h"

#include "MultiColorTest.h"

#include "LinAlg/SparseMatrix2D.h"
#include "LinAlg/operators.h"

#include "LinearSolverLibrary/internal/MultiColor_internal.h"
#include "LinearSolverLibrary/internal/BucketList.h"
#include "LinearSolverLibrary/internal/BucketElement.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;
using namespace internal_NS;


void
MultiColorTest::setUp() {}

void
MultiColorTest::tearDown() {}

namespace {
    bool noElementOnLowerLeft(SparseMatrix2D const & m, int row) {
        return false;
    }

    SparseMatrix2D createMatrix() {
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

        return m;
    }

}

void
MultiColorTest::bucketElementTest() {
    SparseMatrix2D m = createMatrix();

    // extract dependencies
    BucketList bl = MultiColor_internal::extractGraph(m);


    // check dependencies for x_1 (i.e. 1st row of a)
    BucketList::iterator it = bl.findElement(0);
    CPPUNIT_ASSERT_MESSAGE("could not find element", it != bl.end());

    BucketElement belem = *it;
    CPPUNIT_ASSERT_MESSAGE("failure in dependencies", belem.dependsOn(1));
    CPPUNIT_ASSERT_MESSAGE("failure in dependencies", belem.dependsOn(3));
    CPPUNIT_ASSERT_MESSAGE("failure in dependencies", !belem.dependsOn(5));
    CPPUNIT_ASSERT_MESSAGE("failure in dependencies", !belem.dependsOn(8));


    // check dependencies for x_5
    it = bl.findElement(5);
    CPPUNIT_ASSERT_MESSAGE("could not find element", it != bl.end());

    belem = *it;
    CPPUNIT_ASSERT_MESSAGE("failure in dependencies", belem.dependsOn(2));
    CPPUNIT_ASSERT_MESSAGE("failure in dependencies", belem.dependsOn(4));
    CPPUNIT_ASSERT_MESSAGE("failure in dependencies", belem.dependsOn(8));
    CPPUNIT_ASSERT_MESSAGE("failure in dependencies", !belem.dependsOn(1));
    CPPUNIT_ASSERT_MESSAGE("failure in dependencies", !belem.dependsOn(5));

    // check dependencies for x_8
    it = bl.findElement(8);
    CPPUNIT_ASSERT_MESSAGE("could not find element", it != bl.end());

    belem = *it;
    CPPUNIT_ASSERT_MESSAGE("failure in dependencies", belem.dependsOn(5));
    CPPUNIT_ASSERT_MESSAGE("failure in dependencies", belem.dependsOn(7));
    CPPUNIT_ASSERT_MESSAGE("failure in dependencies", !belem.dependsOn(1));
    CPPUNIT_ASSERT_MESSAGE("failure in dependencies", !belem.dependsOn(4));
}

void
MultiColorTest::simpleTest() {
    /* From
     * "Solving the Poisson Problem in Parallel with S.O.R."
     * by Landon Boyd
     */
    SparseMatrix2D m = createMatrix();

    m.print();


    // find independent sets and return new matrix
    SparseMatrix2D new_m = SparseLinearSolverUtil::multicolorDecomposition(m);

    // the first 5 rows should have no elements on the lower-left part
    CPPUNIT_ASSERT_EQUAL_MESSAGE("failure in multicoloring", true, noElementOnLowerLeft(m, 0));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("failure in multicoloring", true, noElementOnLowerLeft(m, 1));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("failure in multicoloring", true, noElementOnLowerLeft(m, 2));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("failure in multicoloring", true, noElementOnLowerLeft(m, 3));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("failure in multicoloring", true, noElementOnLowerLeft(m, 4));
}
