#include "pch.h"

#include "ParallelLinAlgOperationsTest.h"

#include "LinAlg/EntityOperators.h"
#include "LinearSolverLibrary/SparseLinearSolverUtil.h"

#include <thread>
#include <random>
#include <ppl.h>


using namespace EntityReader_NS;
using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;

namespace FS = boost::filesystem;


// use this to work with a small matrix/vector
#define SIMPLE 1
#undef SIMPLE


#ifdef SIMPLE
    std::string const filename = R"(\Develop\SparseMatrixData\bcsstk05\bcsstk05.ar)";
    IMatrix2D::size_type const dim = 153;
#else
    std::string const filename = R"(\Develop\SparseMatrixData\language\language.ar)";
    IMatrix2D::size_type const dim = 399130;
#endif


void
ParallelLinAlgOperationsTest::setUp() {}

void
ParallelLinAlgOperationsTest::tearDown() {}

namespace {

    SparseMatrix2D
    readMatrix() {
        ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename);
        CPPUNIT_ASSERT_MESSAGE("File not found", sm_reader);
        CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());
        SparseMatrix2D const m = sm_reader->get();
        return m;
    }

    Vector
    createVectorOfSize(IMatrix2D::size_type size) {
        Vector result{size};
        std::iota(std::begin(result), std::end(result), 0);
        std::transform(std::begin(result), std::end(result), std::begin(result), [size](double value) {
            return value / size;
        });
        return result;
    }

    Vector
    serialMatrixVectorMultiplication(SparseMatrix2D const & m, Vector const & x) {
        Vector result{x.size()};
        for (IMatrix2D::size_type row = 0; row < x.size(); ++row) {
            double result_row = LinAlg_NS::helper::matrix_vector_mul<Vector>(m, x, row);
            result(row) = result_row;
        }
        return result;
    }

    Vector
    nonChunkedParallelMatrixVectorMultiplication(SparseMatrix2D const & m, Vector const & x) {
        Vector result{ x.size() };
        concurrency::parallel_for(IMatrix2D::size_type{ 0 }, x.size(), [&m, &x, &result](IMatrix2D::size_type row) {
            double result_row = LinAlg_NS::helper::matrix_vector_mul<Vector>(m, x, row);
            result(row) = result_row;
        }, concurrency::static_partitioner());
        return result;
    }

    double
    serialDotProduct(Vector const & v1, Vector const & v2) {
        double result = 0;
        for (IMatrix2D::size_type i = 0; i < v1.size(); ++i) {
            double tmp = v1(i) * v2(i);
            result += tmp;
        }
        return result;
    }

    double
    nonChunkedParallelDotProduct(Vector const & v1, Vector const & v2) {
        using size_type = IMatrix2D::size_type;
        double result = 0;
        concurrency::combinable<double> part_sums([]{
            return 0;
        });
        concurrency::parallel_for(size_type{0}, v1.size(), [&part_sums, &v1, &v2](size_type index) {
            double tmp = v1(index) * v2(index);
            part_sums.local() += tmp;
        });
        result = part_sums.combine(std::plus<double>());
        return result;
    }

    template<typename T>
    std::tuple<T, T>
    getChunkStartEndIndex(T total_size, T nchunks, T current_start_index) {
        auto chunk_size = total_size / nchunks;
        auto diff = total_size % nchunks;

        auto chunk_index = current_start_index / chunk_size;
        auto start_index = current_start_index;
        auto end_size = current_start_index + chunk_size;

        if (diff) {
            if (chunk_index < diff) {
                start_index += chunk_index;
                end_size = start_index + chunk_size + 1;
            }
            else {
                start_index += diff;
                end_size = start_index + chunk_size;
            }
        }
        return std::make_tuple(start_index, end_size);
    }

    template<typename T1, typename T2>
    auto
    getAdjustedSize(T1 total_size, T2 nchunks) -> decltype(T1() - T2()) {
        auto chunk_size = total_size / nchunks;
        auto diff = total_size % nchunks;
        return total_size - diff;
    }

    Vector
    chunkedParallelMatrixVectorMultiplication(SparseMatrix2D const & m, Vector const & x) {
        using size_type = IMatrix2D::size_type;
        Vector result{x.size()};
        size_type numberOfProcessors = std::thread::hardware_concurrency();
        IMatrix2D::size_type chunk_size = x.size() / numberOfProcessors;
        auto size = getAdjustedSize(x.size(), numberOfProcessors);
        concurrency::parallel_for(size_type{0}, size, chunk_size, [&m, &x, &result, numberOfProcessors](size_type row) {
            size_type start_index, end_size;
            std::tie(start_index, end_size) = getChunkStartEndIndex(x.size(), size_type{numberOfProcessors}, row);
            for (size_type i = start_index; i < end_size; ++i) {
                double result_row = LinAlg_NS::helper::matrix_vector_mul<Vector>(m, x, i);
                result(i) = result_row;
            }
        }, concurrency::static_partitioner());
        return result;
    }

    double
    chunkedParallelDotProduct(Vector const & v1, Vector const & v2) {
        using size_type = IMatrix2D::size_type;
        double result = 0;
        concurrency::combinable<double> part_sums([]{
            return 0;
        });
        size_type numberOfProcessors = std::thread::hardware_concurrency();
        IMatrix2D::size_type chunk_size = v1.size() / numberOfProcessors;
        auto size = getAdjustedSize(v1.size(), numberOfProcessors);
        concurrency::parallel_for(size_type{0}, size, chunk_size, [&part_sums, &v1, &v2, numberOfProcessors](size_type index) {
            size_type start_index, end_size;
            std::tie(start_index, end_size) = getChunkStartEndIndex(v1.size(), size_type{numberOfProcessors}, index);
            double part_result = 0;
            for (size_type i = start_index; i < end_size; ++i) {
                double tmp = v1(i) * v2(i);
                part_result += tmp;
            }
            part_sums.local() += part_result;
        }, concurrency::static_partitioner());
        result = part_sums.combine(std::plus<double>());
        return result;
    }

    class HighResTimer {
    public:
        HighResTimer() : start_(boost::chrono::high_resolution_clock::now()) {}
        ~HighResTimer() {
            auto end = boost::chrono::high_resolution_clock::now();
            auto frac = (double)boost::chrono::high_resolution_clock::period::num / boost::chrono::high_resolution_clock::period::den;
            auto d = (end - start_).count() * frac;
            std::cout << std::endl << "Duration: " << d << std::endl;
        }

    private:
        boost::chrono::steady_clock::time_point start_;
    };

}

void
ParallelLinAlgOperationsTest::TestNonChunkedParallelMatrixVectorMultiplication() {
    Vector x = createVectorOfSize(dim);
    SparseMatrix2D const & m = readMatrix();

    Vector serial_result;
    {
        HighResTimer t;
        serial_result = serialMatrixVectorMultiplication(m, x);
    }

    Vector parallel_result;
    {
        HighResTimer t;
        parallel_result = nonChunkedParallelMatrixVectorMultiplication(m, x);
    }
    CPPUNIT_ASSERT_MESSAGE("serial & parallel matrix-vector multiplication mismatch", SparseLinearSolverUtil::isVectorEqual(serial_result, parallel_result, 1E-15));
}

void
ParallelLinAlgOperationsTest::TestChunkedParallelMatrixVectorMultiplication() {
    Vector x = createVectorOfSize(dim);
    SparseMatrix2D const & m = readMatrix();

    Vector serial_result;
    {
        HighResTimer t;
        serial_result = serialMatrixVectorMultiplication(m, x);
    }

    Vector parallel_result;
    {
        HighResTimer t;
        parallel_result = nonChunkedParallelMatrixVectorMultiplication(m, x);
    }
    CPPUNIT_ASSERT_MESSAGE("serial & parallel matrix-vector multiplication mismatch", SparseLinearSolverUtil::isVectorEqual(serial_result, parallel_result, 1E-15));

    {
        HighResTimer t;
        parallel_result = chunkedParallelMatrixVectorMultiplication(m, x);
    }
    CPPUNIT_ASSERT_MESSAGE("serial & parallel matrix-vector multiplication mismatch", SparseLinearSolverUtil::isVectorEqual(serial_result, parallel_result, 1E-15));
}

void
ParallelLinAlgOperationsTest::TestNonChunkedParallelDotProduct() {
    IMatrix2D::size_type dim = 999130;
    Vector v1 = createVectorOfSize(dim);
    Vector v2 = createVectorOfSize(dim);

    double serial_result;
    {
        HighResTimer t;
        serial_result = serialDotProduct(v1, v2);
    }

    double parallel_result;
    {
        HighResTimer t;
        parallel_result = nonChunkedParallelDotProduct(v1, v2);
    }
    // The large tolerance is due to the fact that associativity does not
    // hold when evaluated in parallel.
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("serial & parallel dot product mismatch", serial_result, parallel_result, 1E-7);
}

void
ParallelLinAlgOperationsTest::TestChunkedParallelDotProduct() {
    IMatrix2D::size_type dim = 9199130;
    Vector v1 = createVectorOfSize(dim);
    Vector v2 = createVectorOfSize(dim);

    double serial_result;
    {
        HighResTimer t;
        serial_result = serialDotProduct(v1, v2);
    }

    double parallel_result;
    {
        HighResTimer t;
        parallel_result = chunkedParallelDotProduct(v1, v2);
    }
    // The large tolerance is due to the fact that associativity does not
    // hold when evaluated in parallel.
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("serial & parallel dot product mismatch", serial_result, parallel_result, 1E-7);
}