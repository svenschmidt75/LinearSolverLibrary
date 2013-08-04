#include "pch.h"

#include "GaussTest.h"

#include "LinearSolverLibrary/Gauss.h"
#include "LinearSolverLibrary/SparseLinearSolverUtil.h"

#include "LinAlg/entities.h"
#include "LinAlg/operators.h"


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


void
GaussTest::setUp() {}

void
GaussTest::tearDown() {}

void
GaussTest::Test4by4() {
    Matrix2D A(4, 4);

    A(0, 0) = 2;
    A(0, 1) = 1;
    A(0, 2) = 1;
    A(0, 3) = 0;

    A(1, 0) = 4;
    A(1, 1) = 3;
    A(1, 2) = 3;
    A(1, 3) = 1;

    A(2, 0) = 8;
    A(2, 1) = 7;
    A(2, 2) = 9;
    A(2, 3) = 5;

    A(3, 0) = 6;
    A(3, 1) = 7;
    A(3, 2) = 9;
    A(3, 3) = 8;

    A.print();

    // generate r.h.s.
    Vector rhs(4);
    rhs(0) = 1;
    rhs(1) = 1;
    rhs(2) = 1;
    rhs(3) = 1;

    Matrix2D AInverse(4, 4);
    Vector x(4);
    bool success;
    std::tie(success, AInverse, x) = LinearSolverLibrary_NS::Gauss().solve(A, rhs);


    Vector x_ref(4);
    x_ref = AInverse * rhs;

    CPPUNIT_ASSERT_MESSAGE("mismatch in Gauss solver result", SparseLinearSolverUtil::isVectorEqual(x, x_ref, 1E-10));

}
