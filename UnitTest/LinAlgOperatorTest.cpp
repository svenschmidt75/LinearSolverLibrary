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
    Vector v2 = createVector();
    Vector result(v1.size());

    std::iota(std::begin(v1), std::end(v1), 1);
    std::iota(std::begin(v2), std::end(v2), 10);

    result = 0.1 * (v1 + v2);

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", 1.1, result(0), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", 2.9, result(9), 1E-10);
}

void
LinAlgOperatorTest::vectorExprAddTest() {
    Vector v1 = createVector();
    Vector v2 = createVector();
    Vector v3 = createVector();
    Vector result(v1.size());

    std::iota(std::begin(v1), std::end(v1), 1);
    std::iota(std::begin(v2), std::end(v2), 67);
    std::iota(std::begin(v3), std::end(v3), 978);

    result = (v1 + v2) + v3;

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", 1046, result(0), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", 1061, result(5), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", 1073, result(9), 1E-10);


    result = (v3 + v2) + (0.1 * v3 + v1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", 1143.8, result(0), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", 1159.3, result(5), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("error in vector expression", 1171.7, result(9), 1E-10);
}
