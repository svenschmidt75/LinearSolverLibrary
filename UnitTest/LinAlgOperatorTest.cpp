#include "LinAlgOperatorTest.h"

#include "LinAlg/Vector.h"
#include "LinAlg/operators.h"


using namespace LinAlg_NS;


void
LinAlgOperatorTest::setUp() {}

void
LinAlgOperatorTest::tearDown() {}

namespace {
    
    Vector createVector() {
        Vector v(10);
        v(0) = 10;
        return v;
    }
}

void
LinAlgOperatorTest::scalarVectorMulTest() {
    Vector v1 = createVector();
    Vector result(v1.size());
    result = 0.1 * v1;
    result = 0.1 * (v1 + v1);
    result = (v1 + v1) + v1;
    result = (v1 + v1) + (v1 + v1);
}
