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
    CPPUNIT_TEST(TestFineGridVariablesInfluencedByVariable0WhenAllUndefined);
    CPPUNIT_TEST(TestUndefinedVariablesInfluencedByVariable0AllUndefined);
    CPPUNIT_TEST(TestVariablesInfluencedByVariable4);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void TestFineGridVariablesInfluencedByVariable0WhenAllUndefined();
    void TestUndefinedVariablesInfluencedByVariable0AllUndefined();
    void TestVariablesInfluencedByVariable4();
};
