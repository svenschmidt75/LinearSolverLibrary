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
using namespace EntityReader_NS;

namespace FS = boost::filesystem;


void
MultiColorTest::setUp() {}

void
MultiColorTest::tearDown() {}

namespace {

    void checkISODecomposition(MatrixDecomposition const & m_decomp , SparseMatrix2D const & m) {
        BucketList bl = internal_NS::MultiColor_internal::extractGraph(m);

        // compute color
        internal_NS::MultiColor_internal::computeColor(bl);

        // iterator over all independent sets
        for (auto iso_it = m_decomp.begin(); iso_it != m_decomp.end(); ++iso_it) {
            // color of current independent set
            internal_NS::MultiColor_internal::color_t color = iso_it->first;

            // set of equations
            auto iso_set = iso_it->second;

            // iterate over all elements in the independent set
            for (auto & index : iso_set) {
                auto be_it = bl.findElement(index);
                CPPUNIT_ASSERT_MESSAGE("element not found", be_it != bl.cend());

                BucketElement::Ptr const & be = *be_it;

                CPPUNIT_ASSERT_EQUAL_MESSAGE("color unassigned", true, be->color() != 0);

                /* Check that all dependencies of element 'be' have a different color
                 * than itself.
                 */
                for (auto & dep : *be) {
                    // get the color of the dependent element
                    internal_NS::MultiColor_internal::color_t dep_color = dep->color();

                    CPPUNIT_ASSERT_EQUAL_MESSAGE("color unassigned", true, dep_color != 0);

                    if (dep->prevIndex() != be->prevIndex())
                        CPPUNIT_ASSERT_EQUAL_MESSAGE("color of dependent element must be different", true, dep_color != color);
                }
            }
        }
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
    BucketElement::Ptr e2(new BucketElement(2));

    e0->dependsOn(e1);
    e2->dependsOn(e2);
    bl.insert(e0);
    bl.insert(e1);

    auto decomp = MultiColor_internal::decompose(bl);

    /* e0 and e1 can be compusted in the same color */
    CPPUNIT_ASSERT_EQUAL_MESSAGE("decomposition error", 1ull, decomp.size());
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

    // check the # of independent sets
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected 2 independent sets of equations", 2ull, m_decomp.size());

    auto indep_set_1 = (*(m_decomp.begin())).second;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected 5 equations in independent set 1", 5ull, indep_set_1.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_0 in independent set 1", true, indep_set_1.find(0) != indep_set_1.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_2 in independent set 1", true, indep_set_1.find(2) != indep_set_1.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_4 in independent set 1", true, indep_set_1.find(4) != indep_set_1.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_6 in independent set 1", true, indep_set_1.find(6) != indep_set_1.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_8 in independent set 1", true, indep_set_1.find(8) != indep_set_1.end());

    auto indep_set_2 = (*(++m_decomp.begin())).second;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected 4 equations in independent set 2", 4ull, indep_set_2.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_1 in independent set 2", true, indep_set_2.find(1) != indep_set_2.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_3 in independent set 2", true, indep_set_2.find(3) != indep_set_2.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_5 in independent set 2", true, indep_set_2.find(5) != indep_set_2.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_7 in independent set 2", true, indep_set_2.find(7) != indep_set_2.end());

    checkISODecomposition(m_decomp, m);
}

void
MultiColorTest::SaadFig210Test() {
    /* from Saad, page 67, Fig. 2.10 */
    SparseMatrix2D m = createMatrix2();

//    m.print();

    // find independent sets and return new matrix
    MatrixDecomposition m_decomp = SparseLinearSolverUtil::multicolorDecomposition(m);



    // check the # of independent sets
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected 4 independent sets of equations", 4ull, m_decomp.size());

    auto indep_set_1 = (*(m_decomp.begin())).second;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected 6 equations in independent set 1", 6ull, indep_set_1.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_0 in independent set 1", true, indep_set_1.find(0) != indep_set_1.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_1 in independent set 1", true, indep_set_1.find(1) != indep_set_1.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_2 in independent set 1", true, indep_set_1.find(2) != indep_set_1.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_3 in independent set 1", true, indep_set_1.find(3) != indep_set_1.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_4 in independent set 1", true, indep_set_1.find(4) != indep_set_1.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_5 in independent set 1", true, indep_set_1.find(5) != indep_set_1.end());

    auto indep_set_2 = (*(++m_decomp.begin())).second;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected 3 equations in independent set 2", 3ull, indep_set_2.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_6 in independent set 2", true, indep_set_2.find(6) != indep_set_2.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_9 in independent set 2", true, indep_set_2.find(9) != indep_set_2.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_11 in independent set 2", true, indep_set_2.find(11) != indep_set_2.end());

    auto indep_set_3 = (*(++(++m_decomp.begin()))).second;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected 3 equations in independent set 3", 3ull, indep_set_3.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_7 in independent set 3", true, indep_set_3.find(7) != indep_set_3.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_12 in independent set 3", true, indep_set_3.find(12) != indep_set_3.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_13 in independent set 3", true, indep_set_3.find(13) != indep_set_3.end());

    auto indep_set_4 = (*(++(++(++m_decomp.begin())))).second;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected 3 equations in independent set 4", 3ull, indep_set_4.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_8 in independent set 4", true, indep_set_4.find(8) != indep_set_4.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_10 in independent set 4", true, indep_set_4.find(10) != indep_set_4.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_14 in independent set 4", true, indep_set_4.find(14) != indep_set_4.end());

    checkISODecomposition(m_decomp, m);
}

void
MultiColorTest::VersteegMalalasekeraMultiColorTest() {
    /* from Versteeg, Malalasekera, p. 334.
     * Here with 18 equ. instead of 9.
     */
    // read matrix m
    FS::path filename("\\Develop\\SparseMatrixData\\Versteeg_Malalasekera\\Versteeg_Malalasekera.ar");
    ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("File not found", sm_reader);
    CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());

    SparseMatrix2D const m = sm_reader->get();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 18ull, m.cols());


    // check m is diagonally dominant
    CPPUNIT_ASSERT_MESSAGE("matrix not diagonally dominant", SparseLinearSolverUtil::isStrictlyDiagonallyDominant(m));

//    m.print();

    // find independent sets and return new matrix
    MatrixDecomposition m_decomp = SparseLinearSolverUtil::multicolorDecomposition(m);


    /* I have manually verified the decomposition! */


    // check the # of independent sets
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected 3 independent sets of equations", 3ull, m_decomp.size());

    auto indep_set_1 = (*(m_decomp.begin())).second;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected 8 equations in independent set 1", 8ull, indep_set_1.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_0 in independent set 1", true, indep_set_1.find(0) != indep_set_1.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_2 in independent set 1", true, indep_set_1.find(2) != indep_set_1.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_4 in independent set 1", true, indep_set_1.find(4) != indep_set_1.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_6 in independent set 1", true, indep_set_1.find(6) != indep_set_1.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_8 in independent set 1", true, indep_set_1.find(8) != indep_set_1.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_9 in independent set 1", true, indep_set_1.find(9) != indep_set_1.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_11 in independent set 1", true, indep_set_1.find(11) != indep_set_1.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_13 in independent set 1", true, indep_set_1.find(13) != indep_set_1.end());

    auto indep_set_2 = (*(++m_decomp.begin())).second;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected 8 equations in independent set 2", 8ull, indep_set_2.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_1 in independent set 2", true, indep_set_2.find(1) != indep_set_2.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_3 in independent set 2", true, indep_set_2.find(3) != indep_set_2.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_5 in independent set 2", true, indep_set_2.find(5) != indep_set_2.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_7 in independent set 2", true, indep_set_2.find(7) != indep_set_2.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_10 in independent set 2", true, indep_set_2.find(10) != indep_set_2.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_12 in independent set 2", true, indep_set_2.find(12) != indep_set_2.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_14 in independent set 2", true, indep_set_2.find(14) != indep_set_2.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_16 in independent set 2", true, indep_set_2.find(16) != indep_set_2.end());

    auto indep_set_3 = (*(++(++m_decomp.begin()))).second;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected 2 equations in independent set 3", 2ull, indep_set_3.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_15 in independent set 3", true, indep_set_3.find(15) != indep_set_3.end());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected x_17 in independent set 3", true, indep_set_3.find(17) != indep_set_3.end());

    checkISODecomposition(m_decomp, m);
}

void
MultiColorTest::floridaSherman3Test() {
    FS::path filename("\\Develop\\SparseMatrixData\\sherman3\\sherman3.ar");

    ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());

    SparseMatrix2D const m = sm_reader->get();


    // find independent sets and return new matrix
    MatrixDecomposition m_decomp = SparseLinearSolverUtil::multicolorDecomposition(m);


    // check the # of independent sets
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected 4 independent sets of equations", 4ull, m_decomp.size());

    checkISODecomposition(m_decomp, m);
}

void
MultiColorTest::floridaMemplusTest() {
    FS::path filename("\\Develop\\SparseMatrixData\\memplus\\memplus.ar");

    ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
    CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());

    SparseMatrix2D const m = sm_reader->get();


    // find independent sets and return new matrix
    MatrixDecomposition m_decomp = SparseLinearSolverUtil::multicolorDecomposition(m);


    // check the # of independent sets
    CPPUNIT_ASSERT_EQUAL_MESSAGE("expected 22 independent sets of equations", 22ull, m_decomp.size());

    checkISODecomposition(m_decomp, m);
}
