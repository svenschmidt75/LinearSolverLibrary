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
    bool noElementsOnLowerLeft(SparseMatrix2D const & m, SparseMatrix2D::size_type row, SparseMatrix2D::size_type column_offset) {
        SparseMatrix2D::size_type nrows = m.rows();
        SparseMatrix2D::size_type ncols = m.cols();

        for (SparseMatrix2D::size_type col = column_offset; col < row; ++col) {
            if (m(row, col))
                return false;
        }

        return true;
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

    SparseMatrix2D createMatrix2() {
        /* Matrix from Saad, p. 67, fig. 2.10 */
        SparseMatrix2D m(15);

        m(0, 0) = -4;
        m(0, 6) = 1;
        m(0, 8) = 1;

        m(1,  1) = -4;
        m(1,  7) = 1;
        m(1,  8) = 1;
        m(1, 10) = 1;
        m(1, 12) = 1;

        m(2, 2) = -4;
        m(2, 6) = 1;
        m(2, 7) = 1;
        m(2, 9) = 1;

        m(3, 3) = -4;
        m(3, 11) = 1;
        m(3, 12) = 1;
        m(3, 14) = 1;

        m(4, 4) = -4;
        m(4,  9) = 1;
        m(4, 10) = 1;
        m(4, 11) = 1;
        m(4, 13) = 1;

        m(5, 5) = -4;
        m(5, 13) = 1;
        m(5, 14) = 1;

        m(6, 0) = 1;
        m(6, 2) = 1;
        m(6, 6) = -4;
        m(6, 7) = 1;
        m(6, 8) = 1;

        m(7,  1) = 1;
        m(7,  2) = 1;
        m(7,  6) = 1;
        m(7,  7) = -4;
        m(7,  8) = 1;
        m(7,  9) = 1;
        m(7, 10) = 1;

        m(8, 0) = 1;
        m(8, 1) = 1;
        m(8, 6) = 1;
        m(8, 7) = 1;
        m(8, 8) = -4;

        m(9,  2) = 1;
        m(9,  4) = 1;
        m(9,  7) = 1;
        m(9,  9) = -4;
        m(9, 10) = 1;

        m(10,  1) = 1;
        m(10,  4) = 1;
        m(10,  7) = 1;
        m(10,  9) = 1;
        m(10, 10) = -4;
        m(10, 11) = 1;
        m(10, 12) = 1;

        m(11,  3) = 1;
        m(11,  4) = 1;
        m(11, 10) = 1;
        m(11, 11) = -4;
        m(11, 12) = 1;
        m(11, 13) = 1;
        m(11, 14) = 1;

        m(12,  1) = 1;
        m(12,  3) = 1;
        m(12, 10) = 1;
        m(12, 11) = 1;
        m(12, 12) = -4;

        m(13,  4) = 1;
        m(13,  5) = 1;
        m(13, 11) = 1;
        m(13, 13) = -4;
        m(13, 14) = 1;

        m(14,  3) = 1;
        m(14,  5) = 1;
        m(14, 11) = 1;
        m(14, 13) = 1;
        m(14, 14) = -4;

        m.finalize();

        return m;
    }

}

void
MultiColorTest::bucketListDecompositionTest() {
    BucketList bl;

    BucketElement::Ptr e0(new BucketElement(0));
    BucketElement::Ptr e1(new BucketElement(1));

    e0->dependsOn(e1);
    bl.insert(e0);
    bl.insert(e1);


    auto decomp = MultiColor_internal::decompose(bl);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("decomposition error", 2ull, decomp.size());
}

void
MultiColorTest::bucketElementTest() {
    SparseMatrix2D m = createMatrix();

    // extract dependencies
    BucketList bl = MultiColor_internal::extractGraph(m);


    // check dependencies for x_1 (i.e. 1st row of a)
    BucketList::iterator it = bl.findElement(0);
    CPPUNIT_ASSERT_MESSAGE("could not find element", it != bl.end());

    BucketElement::Ptr const & belem1 = *it;
    CPPUNIT_ASSERT_MESSAGE("failure in dependencies", belem1->findDependency(1));
    CPPUNIT_ASSERT_MESSAGE("failure in dependencies", belem1->findDependency(3));
    CPPUNIT_ASSERT_MESSAGE("failure in dependencies", !belem1->findDependency(5));
    CPPUNIT_ASSERT_MESSAGE("failure in dependencies", !belem1->findDependency(8));


    // check dependencies for x_5
    it = bl.findElement(5);
    CPPUNIT_ASSERT_MESSAGE("could not find element", it != bl.end());

    BucketElement::Ptr const & belem5 = *it;
    CPPUNIT_ASSERT_MESSAGE("failure in dependencies", belem5->findDependency(2));
    CPPUNIT_ASSERT_MESSAGE("failure in dependencies", belem5->findDependency(4));
    CPPUNIT_ASSERT_MESSAGE("failure in dependencies", belem5->findDependency(8));
    CPPUNIT_ASSERT_MESSAGE("failure in dependencies", !belem5->findDependency(1));
    CPPUNIT_ASSERT_MESSAGE("failure in dependencies", !belem5->findDependency(7));

    // check dependencies for x_8
    it = bl.findElement(8);
    CPPUNIT_ASSERT_MESSAGE("could not find element", it != bl.end());

    BucketElement::Ptr const & belem8 = *it;
    CPPUNIT_ASSERT_MESSAGE("failure in dependencies", belem8->findDependency(5));
    CPPUNIT_ASSERT_MESSAGE("failure in dependencies", belem8->findDependency(7));
    CPPUNIT_ASSERT_MESSAGE("failure in dependencies", !belem8->findDependency(1));
    CPPUNIT_ASSERT_MESSAGE("failure in dependencies", !belem8->findDependency(4));
}

void
MultiColorTest::simpleTest() {
    /* From
     * "Solving the Poisson Problem in Parallel with S.O.R."
     * by Landon Boyd
     */
    SparseMatrix2D m = createMatrix();

//    m.print();


    // find independent sets and return new matrix
    MatrixDecomposition m_decomp = SparseLinearSolverUtil::multicolorDecomposition(m);
    SparseMatrix2D const & new_m = m_decomp.matrix();

//    new_m.print();

    // the first 5 rows should have no elements on the lower-left part
    CPPUNIT_ASSERT_EQUAL_MESSAGE("failure in multicoloring", true, noElementsOnLowerLeft(new_m, 0, 0));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("failure in multicoloring", true, noElementsOnLowerLeft(new_m, 1, 0));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("failure in multicoloring", true, noElementsOnLowerLeft(new_m, 2, 0));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("failure in multicoloring", true, noElementsOnLowerLeft(new_m, 3, 0));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("failure in multicoloring", true, noElementsOnLowerLeft(new_m, 4, 0));

    // the last 4 rows should have no elements on the lower-left part for column > 5
    CPPUNIT_ASSERT_EQUAL_MESSAGE("failure in multicoloring", true, noElementsOnLowerLeft(new_m, 5, 5));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("failure in multicoloring", true, noElementsOnLowerLeft(new_m, 6, 5));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("failure in multicoloring", true, noElementsOnLowerLeft(new_m, 7, 5));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("failure in multicoloring", true, noElementsOnLowerLeft(new_m, 8, 5));

    // check # of independent sets




}

void
MultiColorTest::SaadFig210Test() {
    /* from Saag, page 67, Fig. 2.10 */
    SparseMatrix2D m = createMatrix2();

    m.print();

#if 0
    // find independent sets and return new matrix
    MatrixDecomposition m_decomp = SparseLinearSolverUtil::multicolorDecomposition(m);
    SparseMatrix2D const & new_m = m_decomp.matrix();

    new_m.print();

    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected 4 sets of independent equations", std::iterator_traits<decltype(m_decomp.cbegin())>::difference_type(4), std::distance(m_decomp.cbegin(), m_decomp.cend()));

    auto ips = *(m_decomp.cbegin());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected 4 equations in independent set 1", 4ull, (m_decomp.cbegin()).size());

#endif
}
