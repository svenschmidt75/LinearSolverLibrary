/*
 * Name  : spiral
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 08/03/2013
 */
#pragma once

#include <cppunit/extensions/HelperMacros.h>

#include "LinAlg/entities.h"


class spiralTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(spiralTest);
    CPPUNIT_TEST(spiralMINRESTest);
    CPPUNIT_TEST(spiralMINRESLanProTest);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void spiralMINRESTest();
    void spiralMINRESLanProTest();

private:
    void initMatrix();
    void matrixIsDiagonallyDominant();
    void matrixIsSymmetric();

private:
    LinAlg_NS::SparseMatrix2D m_;
    LinAlg_NS::Vector b_;
    LinAlg_NS::Vector x_ref_;
};
