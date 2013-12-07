/*
 * Name  : FIDAP_EX5_Test
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 12/07/2013
 */
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class FIDAP_EX5_Test : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(FIDAP_EX5_Test);
    CPPUNIT_TEST(CGTest);
    CPPUNIT_TEST(BiCGTest);
    CPPUNIT_TEST(GMRESTest);
    CPPUNIT_TEST(TFQMRTest);
    CPPUNIT_TEST(GCRWithNoRestartTest);
    CPPUNIT_TEST(GCRWithRestartTest);
    CPPUNIT_TEST(MINRESTest);
    CPPUNIT_TEST(MINRESLanProTest);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void CGTest();
    void BiCGTest();
    void GMRESTest();
    void TFQMRTest();
    void GCRWithNoRestartTest();
    void GCRWithRestartTest();
    void MINRESTest();
    void MINRESLanProTest();

private:
    void initMatrix();

private:
    LinAlg_NS::SparseMatrix2D m_;
    LinAlg_NS::Vector         b_;
    LinAlg_NS::Vector         x_ref_;
};
