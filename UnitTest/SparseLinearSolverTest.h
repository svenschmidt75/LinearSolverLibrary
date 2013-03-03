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
    CPPUNIT_TEST(bcsstk05SORTest);
    CPPUNIT_TEST(bcsstk05CGTest);
#ifdef INTEGRATION_TEST
    CPPUNIT_TEST(sts4098SORTest);
    CPPUNIT_TEST(OffshoreSORTest);
#endif
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void VersteegMalalasekeraSORTest();
    void VersteegMalalasekeraSORMultiColorTest();
    void VersteegMalalasekeraCGTest();
    void bcsstk05SORTest();
    void bcsstk05CGTest();
    void sts4098SORTest();
    void OffshoreSORTest();
};


