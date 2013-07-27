/*
 * Name  : VersteegMalalasekeraTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 07/27/2013
 */
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class VersteegMalalasekeraTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(VersteegMalalasekeraTest);
    CPPUNIT_TEST(SORTest);
    CPPUNIT_TEST(SORMultiColorTest);
    CPPUNIT_TEST(CGTest);
    CPPUNIT_TEST(BiCGTest);
    CPPUNIT_TEST(BiCGSTABTest);
    CPPUNIT_TEST(GMRESTest);
    CPPUNIT_TEST(MINRESTest);
    CPPUNIT_TEST(TFQMRTest);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void matrixIsDiagonallyDominant();
    void SORTest();
    void SORMultiColorTest();
    void CGTest();
    void BiCGTest();
    void BiCGSTABTest();
    void GMRESTest();
    void MINRESTest();
    void MINRESLanProTest();
    void TFQMRTest();

private:
    void initMatrix();
    void initRHS();
    void initReferenceVector();

private:
    LinAlg_NS::SparseMatrix2D m_;
    LinAlg_NS::Vector b_;
    LinAlg_NS::Vector x_ref_;
};
