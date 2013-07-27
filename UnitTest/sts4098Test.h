/*
 * Name  : sts4098Test
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 07/27/2013
 */
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class sts4098Test : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(sts4098Test);
    CPPUNIT_TEST(SORTest);
    CPPUNIT_TEST(MINRESTest);
    CPPUNIT_TEST(MINRESLanProTest);
    CPPUNIT_TEST(GMRESTest);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void SORTest();
    void MINRESTest();
    void MINRESLanProTest();
    void GMRESTest();

private:
    void initMatrix();
    void initRHS();
    void matrixIsDiagonallyDominant();

private:
    LinAlg_NS::SparseMatrix2D m_;
    LinAlg_NS::Vector b_;
    LinAlg_NS::Vector x_ref_;
};
