/*
 * Name  : MultiColorTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 09/22/2012
 */
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class MultiColorTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(MultiColorTest);
    CPPUNIT_TEST(bucketListDecompositionTest);
    CPPUNIT_TEST(bucketElementTest);
    CPPUNIT_TEST(simpleTest);
    CPPUNIT_TEST(SaadFig210Test);
    CPPUNIT_TEST(VersteegMalalasekeraMultiColorTest);
#ifdef INTEGRATION_TEST
    CPPUNIT_TEST(floridaSherman3Test);
    CPPUNIT_TEST(floridaMemplusTest);
#endif
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void bucketListDecompositionTest();
    void bucketElementTest();
    void simpleTest();
    void SaadFig210Test();
    void VersteegMalalasekeraMultiColorTest();
    void floridaSherman3Test();
    void floridaMemplusTest();
};
