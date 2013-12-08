#include "pch.h"

#include "ParallelLinAlgOperationsTest.h"

#include "LinAlg/EntityOperators.h"
#include "LinearSolverLibrary/SparseLinearSolverUtil.h"

#include <thread>
#include <ppl.h>


using namespace EntityReader_NS;
using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;

namespace FS = boost::filesystem;


void
ParallelLinAlgOperationsTest::setUp() {}

void
ParallelLinAlgOperationsTest::tearDown() {}

namespace {

    SparseMatrix2D
    readMatrix() {
        FS::path filename("\\Develop\\SparseMatrixData\\bcsstk05\\bcsstk05.ar");
//        FS::path filename("\\Develop\\SparseMatrixData\\language\\language.ar");
        ISparseMatrixReader::Ptr sm_reader = SparseMatrixReaderCreator::create(filename.string());
        CPPUNIT_ASSERT_MESSAGE("File not found", sm_reader);
        CPPUNIT_ASSERT_MESSAGE("error reading sparse matrix data", sm_reader->read());
        SparseMatrix2D const m = sm_reader->get();
        return m;
    }

    Vector
    createVectorOfSize(IMatrix2D::size_type size) {
        Vector result{size};
        std::iota(std::begin(result), std::end(result), 1);
        return result;
    }

    Vector
    SerialMatrixVectorMultiplication( SparseMatrix2D const & m, Vector const & x) {
        Vector result{x.size()};
        for (IMatrix2D::size_type row = 0; row < x.size(); ++row) {
            double result_row = LinAlg_NS::helper::matrix_vector_mul<Vector>(m, x, row);
            result(row) = result_row;
        }
        return result;
    }

    Vector
    ParallelMatrixVectorMultiplication(SparseMatrix2D const & m, Vector const & x) {
        Vector result{x.size()};
        concurrency::parallel_for(IMatrix2D::size_type{0}, x.size(), [&m, &x, &result](IMatrix2D::size_type row) {
            double result_row = LinAlg_NS::helper::matrix_vector_mul<Vector>(m, x, row);
            result(row) = result_row;
        }, concurrency::static_partitioner());
        return result;
    }

    /*

    Vector
    ParallelMatrixVectorMultiplication2(SparseMatrix2D const & m, Vector const & x) {
    int numberOfProcessors = std::thread::hardware_concurrency();
    numberOfProcessors = 8;

    int size = 23;

    int size_per_chunk = size / numberOfProcessors;
    int diff = size % numberOfProcessors;
    if (diff > numberOfProcessors) {
    size_per_chunk++;
    diff -= numberOfProcessors;
    }

    for (int i = 0; i < size - diff; i += size_per_chunk) {

    int start_index = i;
    int end_size = i + size_per_chunk;
    int chunk_index = i / size_per_chunk;
    int i_copy = i;
    if (diff) {
    if (chunk_index < diff) {
    i_copy += chunk_index;
    end_size = i_copy + size_per_chunk + 1;
    }
    else {
    i_copy += diff;
    end_size = i_copy + size_per_chunk;
    }
    }

    std::cout << "Chunk index: " << chunk_index << std::endl;
    std::cout << "Start index: " << start_index << std::endl;
    std::cout << "Adjusted start index: " << i_copy << std::endl;
    std::cout << "End index: " << i + size_per_chunk << std::endl;
    std::cout << "Adjusted end index: " << end_size << std::endl;

    for (int j = i_copy; j < end_size; ++j) {
    std::cout << "i, j: " << i_copy << ", " << j << std::endl;
    }

    }

    return Vector{1};
    }

    */

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

    Vector
    ParallelMatrixVectorMultiplication2(SparseMatrix2D const & m, Vector const & x) {
        int numberOfProcessors = std::thread::hardware_concurrency();
        numberOfProcessors = 3;
        int size = 19;

        int size_per_chunk = size / numberOfProcessors;
        int diff = size % numberOfProcessors;

        for (int i = 0; i < size - diff; i += size_per_chunk) {

            int chunk_index = i / size_per_chunk;
            int start_index, end_size;
            std::tie(start_index, end_size) = getChunkStartEndIndex(size, numberOfProcessors, i);

            std::cout << "Chunk index: " << chunk_index << std::endl;
            for (int j = start_index; j < end_size; ++j) {
                std::cout << "i, j: " << start_index << ", " << j << std::endl;
            }

        }

        return Vector{1};
    }

    class HighResTimer {
    public:
        HighResTimer() : start_(boost::chrono::high_resolution_clock::now()) {}
        ~HighResTimer() {
            auto end = boost::chrono::high_resolution_clock::now();
            auto d = (end - start_).count() * ((double)boost::chrono::high_resolution_clock::period::num / boost::chrono::high_resolution_clock::period::den);
            std::cout << std::endl << "Duration: " << d << std::endl;
        }

    private:
        boost::chrono::steady_clock::time_point start_;
    };

}

void
ParallelLinAlgOperationsTest::TestParallelMatrixVectorMultiplication() {
    IMatrix2D::size_type const dim = 153;
//    IMatrix2D::size_type const dim = 399130;
    Vector x = createVectorOfSize(dim);
    SparseMatrix2D const & m = readMatrix();

    Vector serial_result;
    {
        HighResTimer t;
        serial_result = SerialMatrixVectorMultiplication(m, x);
    }

    Vector parallel_result;
    {
        HighResTimer t;
        parallel_result = ParallelMatrixVectorMultiplication(m, x);
    }
    CPPUNIT_ASSERT_MESSAGE("mismatch in linear solver result", SparseLinearSolverUtil::isVectorEqual(serial_result, parallel_result, 1E-15));
}

void
ParallelLinAlgOperationsTest::TestParallelChunkedMatrixVectorMultiplication() {
    IMatrix2D::size_type const dim = 153;
    //    IMatrix2D::size_type const dim = 399130;
    Vector x = createVectorOfSize(dim);
    SparseMatrix2D const & m = readMatrix();

    Vector parallel_result2;
    {
        HighResTimer t;
        parallel_result2 = ParallelMatrixVectorMultiplication2(m, x);
    }
}
