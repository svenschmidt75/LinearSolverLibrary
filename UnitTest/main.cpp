#include "pch.h"

#include "LinAlgOperatorTest.h"
#include "FloridaSparseMatrixReaderTest.h"
#include "FloridaVectorReaderTest.h"
#include "SparseLinearSolverTest.h"
#include "MultiColorTest.h"
#include "LinAlgTest.h"
#include "UTMatrixTest.h"
#include "UHMatrixTest.h"
#include "VersteegMalalasekeraTest.h"
#include "bcsstk05Test.h"
#include "sts4098Test.h"
#include "fs_680_1_Test.h"
#include "spiralTest.h"
#include "GaussJordanTest.h"
#include "LUDecompositionTest.h"
#include "SparseMatrixIteratorTest.h"
#include "MatrixStencilTest.h"
#include "AMGStandardCoarseningTest.h"
#include "AMGVariableCardinalityTest.h"


CPPUNIT_TEST_SUITE_REGISTRATION(AMGStandardCoarseningTest);
CPPUNIT_TEST_SUITE_REGISTRATION(AMGVariableCardinalityTest);
#if 0
CPPUNIT_TEST_SUITE_REGISTRATION(MatrixStencilTest);
CPPUNIT_TEST_SUITE_REGISTRATION(SparseMatrixIteratorTest);
CPPUNIT_TEST_SUITE_REGISTRATION(LUDecompositionTest);
CPPUNIT_TEST_SUITE_REGISTRATION(VersteegMalalasekeraTest);
CPPUNIT_TEST_SUITE_REGISTRATION(GaussJordanTest);
#ifdef INTEGRATION_TEST
CPPUNIT_TEST_SUITE_REGISTRATION(spiralTest);
CPPUNIT_TEST_SUITE_REGISTRATION(bcsstk05Test);
CPPUNIT_TEST_SUITE_REGISTRATION(sts4098Test);
CPPUNIT_TEST_SUITE_REGISTRATION(fs_680_1_Test);
#endif
CPPUNIT_TEST_SUITE_REGISTRATION(SparseLinearSolverTest);
CPPUNIT_TEST_SUITE_REGISTRATION(UHMatrixTest);
CPPUNIT_TEST_SUITE_REGISTRATION(UTMatrixTest);
CPPUNIT_TEST_SUITE_REGISTRATION(LinAlgTest);
CPPUNIT_TEST_SUITE_REGISTRATION(MultiColorTest);
CPPUNIT_TEST_SUITE_REGISTRATION(FloridaVectorReaderTest);
CPPUNIT_TEST_SUITE_REGISTRATION(FloridaSparseMatrixReaderTest);
CPPUNIT_TEST_SUITE_REGISTRATION(LinAlgOperatorTest);
#endif

int main(int /*argc*/, char ** /*argv*/) {
    // informs test-listener about testresults
    CPPUNIT_NS::TestResult testresult;

    // register listener for collecting the test-results
    CPPUNIT_NS::TestResultCollector collectedresults;
    testresult.addListener(&collectedresults);

    // register listener for per-test progress output
    CPPUNIT_NS::BriefTestProgressListener progress;
    testresult.addListener(&progress);

    // insert test-suite at test-runner by registry
    CPPUNIT_NS::TestRunner testrunner;
    testrunner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
    testrunner.run(testresult);

    // output results in compiler-format
    CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
    compileroutputter.write ();

    // return 0 if tests were successful
    return collectedresults.wasSuccessful() ? 0 : 1;
}
