#include "LinAlgOperatorTest.h"

#include "LinAlg/Vector.h"
#include "LinAlg/operators.h"

#include <numeric>


using namespace LinAlg_NS;


void
LinAlgOperatorTest::setUp() {}

void
LinAlgOperatorTest::tearDown() {}

namespace {
    
    Vector createVector() {
        Vector v(10);
        return v;
    }
}

void
LinAlgOperatorTest::scalarVectorMulTest() {
    Vector v1 = createVector();
    Vector result(v1.size());

    std::iota(std::begin(v1), std::end(v1), 1);

    result = 0.1 * v1;

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", 0.1, result(0), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", 1.0, result(9), 1E-10);
}

void
LinAlgOperatorTest::scalarVectorExprMulTest() {
    Vector v1 = createVector();
    Vector result(v1.size());


    result = 0.1 * v1;
    result = 0.1 * (v1 + v1);
    result = (v1 + v1) + v1;
    result = (v1 + v1) + (v1 + v1);
}
