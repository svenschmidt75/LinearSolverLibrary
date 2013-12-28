/*
 * Name  : OffshoreTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 12/02/2013
 */
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class OffshoreTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(OffshoreTest);
    CPPUNIT_TEST(offshoreGCTest);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void offshoreMINRESTest();
    void offshoreMINRESLanProTest();
    void offshoreGMRESTest();
    void offshoreGCTest();

private:
    void initMatrix();
    void initRHS();
    void matrixIsDiagonallyDominant();

private:
    LinAlg_NS::SparseMatrix2D m_;
    LinAlg_NS::Vector         b_;
    LinAlg_NS::Vector         x_ref_;
};
