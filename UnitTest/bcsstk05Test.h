/*
 * Name  : bcsstk05Test
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 07/27/2013
 * 
 * Cond(A) = 14281.1
 */
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class bcsstk05Test : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(bcsstk05Test);
    CPPUNIT_TEST(SORTest);
    CPPUNIT_TEST(CGTest);
    CPPUNIT_TEST(BiCGTest);
    CPPUNIT_TEST(BiCGSTABTest);
    CPPUNIT_TEST(MINRESTest);
    CPPUNIT_TEST(MINRESLanProTest);
    CPPUNIT_TEST(GCRWithNoRestartTest);
    CPPUNIT_TEST(GCRWithRestartTest);
    CPPUNIT_TEST(AMGVCycleTest);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void SORTest();
    void CGTest();
    void BiCGTest();
    void BiCGSTABTest();
    void MINRESTest();
    void MINRESLanProTest();
    void GCRWithNoRestartTest();
    void GCRWithRestartTest();
    void AMGVCycleTest();

private:
    void initMatrix();
    void matrixIsDiagonallyDominant();
    void matrixIsSymmetric();

private:
    LinAlg_NS::SparseMatrix2D m_;
    LinAlg_NS::Vector         b_;
    LinAlg_NS::Vector         x_ref_;
};
