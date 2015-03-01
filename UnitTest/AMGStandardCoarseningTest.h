/*
* Name  : AMGStandardCoarseningTest
* Path  :
* Use   :
* Author: Sven Schmidt
* Date  : 11/09/2013
*/
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class AMGStandardCoarseningTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(AMGStandardCoarseningTest);
    CPPUNIT_TEST(TestSplittingForSimple5ptStencil);
    CPPUNIT_TEST(TestSplittingForSimple9ptStencil);
    CPPUNIT_TEST(TestSplittingForNontrivial9ptStencil);
    CPPUNIT_TEST(TestSplittingForSimple9ptStencilWithPeriodicBoundaryConditions);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void TestSplittingForSimple5ptStencil();
    void TestSplittingForSimple9ptStencil();
    void TestSplittingForNontrivial9ptStencil();
    void TestSplittingForSimple9ptStencilWithPeriodicBoundaryConditions();
};
