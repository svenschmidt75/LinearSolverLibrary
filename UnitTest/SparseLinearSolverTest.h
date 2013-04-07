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
    CPPUNIT_TEST(VersteegMalalasekeraSORTest);
    CPPUNIT_TEST(VersteegMalalasekeraSORMultiColorTest);
    CPPUNIT_TEST(VersteegMalalasekeraCGTest);
    CPPUNIT_TEST(VersteegMalalasekeraBiCGTest);
    CPPUNIT_TEST(VersteegMalalasekeraBiCGSTABTest);
    CPPUNIT_TEST(VersteegMalalasekeraGMRESTest);
#ifdef INTEGRATION_TEST
    CPPUNIT_TEST(bcsstk05SORTest);
    CPPUNIT_TEST(bcsstk05CGTest);
    CPPUNIT_TEST(bcsstk05BiCGTest);
    CPPUNIT_TEST(bcsstk05BiCGSTABTest);
    CPPUNIT_TEST(sts4098SORTest);
    CPPUNIT_TEST(OffshoreSORTest);
    CPPUNIT_TEST(fs_680_1CGTest);
    CPPUNIT_TEST(fs_680_1BiCGTest);
    CPPUNIT_TEST(fs_680_1BiCGSTABTest);
#endif
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void VersteegMalalasekeraSORTest();
    void VersteegMalalasekeraSORMultiColorTest();
    void VersteegMalalasekeraCGTest();
    void VersteegMalalasekeraBiCGTest();
    void VersteegMalalasekeraBiCGSTABTest();
    void VersteegMalalasekeraGMRESTest();
    void bcsstk05SORTest();
    void bcsstk05CGTest();
    void bcsstk05BiCGTest();
    void bcsstk05BiCGSTABTest();
    void sts4098SORTest();
    void OffshoreSORTest();
    void fs_680_1CGTest();
    void fs_680_1BiCGTest();
    void fs_680_1BiCGSTABTest();
};


