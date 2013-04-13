#include "pch.h"

#include "LinAlgOperatorTest.h"
#include "FloridaSparseMatrixReaderTest.h"
#include "FloridaVectorReaderTest.h"
#include "SparseLinearSolverTest.h"
#include "MultiColorTest.h"
#include "LinAlgTest.h"
#include "UTMatrixTest.h"
#include "UHMatrixTest.h"


CPPUNIT_TEST_SUITE_REGISTRATION(UHMatrixTest);
CPPUNIT_TEST_SUITE_REGISTRATION(UTMatrixTest);
CPPUNIT_TEST_SUITE_REGISTRATION(LinAlgTest);
CPPUNIT_TEST_SUITE_REGISTRATION(MultiColorTest);
CPPUNIT_TEST_SUITE_REGISTRATION(SparseLinearSolverTest);
CPPUNIT_TEST_SUITE_REGISTRATION(FloridaVectorReaderTest);
CPPUNIT_TEST_SUITE_REGISTRATION(FloridaSparseMatrixReaderTest);
CPPUNIT_TEST_SUITE_REGISTRATION(LinAlgOperatorTest);


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
