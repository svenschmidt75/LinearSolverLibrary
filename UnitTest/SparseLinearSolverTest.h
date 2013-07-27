/*
 * Name  : SparseLinearSolverTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 09/16/2012
 */
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class SparseLinearSolverTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(SparseLinearSolverTest);
    CPPUNIT_TEST(SimonExample1Test);
#ifdef INTEGRATION_TEST
    CPPUNIT_TEST(SimonExample1Test);
    CPPUNIT_TEST(OffshoreSORTest);
    CPPUNIT_TEST(spiralMINRESTest);
    CPPUNIT_TEST(spiralMINRESLanProTest);
#endif
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void SimonExample1Test();
    void OffshoreSORTest();
    void spiralMINRESTest();
    void spiralMINRESLanProTest();
};
