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
#ifdef INTEGRATION_TEST
    CPPUNIT_TEST(sts4098SORTest);
#endif
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void VersteegMalalasekeraSORTest();
    void VersteegMalalasekeraSORMultiColorTest();
    void sts4098SORTest();
};


