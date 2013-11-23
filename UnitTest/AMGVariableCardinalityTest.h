/*
* Name  : AMGVariableCardinalityTest
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 10/19/2013
*/
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class AMGVariableCardinalityTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(AMGVariableCardinalityTest);
    CPPUNIT_TEST(TestCardinalityOfCornerGridVariable);
    CPPUNIT_TEST(TestCardinalityOfBoundaryGridVariable);
    CPPUNIT_TEST(TestCardinalityOfBoundaryGridVariableWithUnrelatedChanges);
    CPPUNIT_TEST(TestCardinalityOfCenterGridVariable);
    CPPUNIT_TEST(TestThatCardinalityIsSameForAllVariablesWithPeriodicBoundaryConditions);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void TestCardinalityOfCornerGridVariable();
    void TestCardinalityOfBoundaryGridVariable();
    void TestCardinalityOfBoundaryGridVariableWithUnrelatedChanges();
    void TestCardinalityOfCenterGridVariable();
    void TestThatCardinalityIsSameForAllVariablesWithPeriodicBoundaryConditions();
};
