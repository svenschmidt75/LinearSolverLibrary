/*
 * Name  : fs_680_1_Test
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 07/27/2013
 *
 * FS 680 1: Chemical kinetics problems
 * RCHEM radiation chemistry study -- 1st output time step
 * from set FACSIMILE, from the Harwell-Boeing Collection
 * 
 * Note: fs_680_1 is a 680x680 real unsymmetric matrix which
 * is not diagonally dominant.
 */
#pragma once

#include <cppunit/extensions/HelperMacros.h>

#include "LinAlg/entities.h"


class fs_680_1_Test : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(fs_680_1_Test);
//     CPPUNIT_TEST(CGTest);
//     CPPUNIT_TEST(BiCGTest);
//     CPPUNIT_TEST(BiCGSTABTest);
//     CPPUNIT_TEST(GMRESTest);
//     CPPUNIT_TEST(TFQMRTest);
    CPPUNIT_TEST(GCRWithNoRestartTest);
//    CPPUNIT_TEST(GCRWithRestartTest);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void CGTest();
    void BiCGTest();
    void BiCGSTABTest();
    void GMRESTest();
    void TFQMRTest();
    void GCRWithNoRestartTest();
    void GCRWithRestartTest();

private:
    void initMatrix();
    void matrixIsDiagonallyDominant();

private:
    LinAlg_NS::SparseMatrix2D m_;
    LinAlg_NS::Vector b_;
    LinAlg_NS::Vector x_ref_;
};
