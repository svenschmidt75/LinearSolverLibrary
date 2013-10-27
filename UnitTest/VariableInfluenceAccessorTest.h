/*
* Name  : VariableInfluenceAccessorTest
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 10/26/2013
*/
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class VariableInfluenceAccessorTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(VariableInfluenceAccessorTest);
    CPPUNIT_TEST(TestFineGridVariablesInfluencedByCornerGridVariableWhenAllUndefined);
    CPPUNIT_TEST(TestUndefinedVariablesInfluencedByCornerGridVariableAllUndefined);
    CPPUNIT_TEST(TestVariablesInfluencedByCenterVariable);
    CPPUNIT_TEST(TestFineGridVariablesInfluencedByBoundaryCenterVariableWhenAllUndefined);
    CPPUNIT_TEST(TestInfluencedByCenterVariable);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void TestFineGridVariablesInfluencedByCornerGridVariableWhenAllUndefined();
    void TestUndefinedVariablesInfluencedByCornerGridVariableAllUndefined();
    void TestVariablesInfluencedByCenterVariable();
    void TestFineGridVariablesInfluencedByBoundaryCenterVariableWhenAllUndefined();
    void TestInfluencedByCenterVariable();
};
