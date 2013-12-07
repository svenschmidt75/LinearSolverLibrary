#include "pch.h"

#include "OffshoreTest.h"

#include "LinAlg/EntityOperators.h"

#include "LinearSolverLibrary/ConjugateGradientMethods.hpp"


using namespace EntityReader_NS;
using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;

namespace FS = boost::filesystem;



namespace {

    SparseMatrix2D
    readMatrix(SparseMatrix2D & in) {
        FS::path filename("\\Develop\\SparseMatrixData\\offshore\\offshore.ar");
        ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
        CPPUNIT_ASSERT_MESSAGE("File not found", sm_reader);
        CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());
        SparseMatrix2D const m = sm_reader->get();
        CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 259789ull, in.rows());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("error in number of columns", 259789ull, in.cols());
        return m;
    }

    Vector
    readRHS() {
        FS::path filename = "\\Develop\\SparseMatrixData\\offshore\\offshore_b.ar";
        IVectorReader::Ptr b_reader = VectorReaderCreator::create(filename.string());
        CPPUNIT_ASSERT_MESSAGE("error reading vector data", b_reader->read());
        Vector const b = b_reader->get();
        CPPUNIT_ASSERT_EQUAL_MESSAGE("error in vector size", 259789ull, b.size());
        return b;
    }

}

void
OffshoreTest::setUp() {
    initMatrix();
    initRHS();
}

void
OffshoreTest::initMatrix() {
    m_ = readMatrix(m_);
}

void
OffshoreTest::initRHS() {
    b_ = std::move(readRHS());
}

void
OffshoreTest::tearDown() {}

void
OffshoreTest::matrixIsDiagonallyDominant() {
    CPPUNIT_ASSERT_MESSAGE("matrix should not be diagonally dominant", !SparseLinearSolverUtil::isStrictlyDiagonallyDominant(m_));
}

namespace {

    void
    checkThatMatrixTimesVectorYieldsRightHandSide(SparseMatrix2D const & m, Vector const & b, Vector const & x, double tolerance) {
        // compute A x and check that = b
        Vector tmp(x.size());
        tmp = m * x;
        CPPUNIT_ASSERT_MESSAGE("mismatch in linear solver result", SparseLinearSolverUtil::isVectorEqual(tmp, b, tolerance));
    }

}

namespace {

    class HighResTimer {
    public:
        HighResTimer() : start_(boost::chrono::high_resolution_clock::now()) {}
        ~HighResTimer() {
            auto end = boost::chrono::high_resolution_clock::now();
            auto d = boost::chrono::duration_cast<boost::chrono::milliseconds>(end - start_);
            std::cout << std::endl << "Duration: " << d << std::endl;
        }

    private:
        boost::chrono::steady_clock::time_point start_;
    };

}


void
OffshoreTest::offshoreGCTest() {
    bool success;
    Vector x(b_.size());

    SparseMatrix2D::size_type iterations;
    double tol;

    {
        HighResTimer t;

        // CG cannot solve this matrix as it is not symmetric
        std::tie(success, x, iterations, tol) = ConjugateGradientMethods::CG(m_, b_, 10000);
    }
    CPPUNIT_ASSERT_MESSAGE("CG should have succeeded to solve linear system", success);
}
