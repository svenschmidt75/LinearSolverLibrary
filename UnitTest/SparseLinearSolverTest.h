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
    CPPUNIT_TEST(fs_680_1CGTest);
    CPPUNIT_TEST(fs_680_1BiCGTest);
    CPPUNIT_TEST(fs_680_1BiCGSTABTest);
    CPPUNIT_TEST(fs_680_1GMRESTest);
    CPPUNIT_TEST(fs_680_1TFQMRTest);
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
    void fs_680_1CGTest();
    void fs_680_1BiCGTest();
    void fs_680_1BiCGSTABTest();
    void fs_680_1GMRESTest();
    void fs_680_1TFQMRTest();
    void spiralMINRESTest();
    void spiralMINRESLanProTest();
};
