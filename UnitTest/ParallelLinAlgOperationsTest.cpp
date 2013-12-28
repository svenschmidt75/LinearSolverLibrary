#include "pch.h"

#include "ParallelLinAlgOperationsTest.h"

#include "LinAlg/MatrixStencil.hpp"


using namespace EntityReader_NS;
using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;

namespace FS = boost::filesystem;


// use this to work with a small matrix/vector
#define SIMPLE 1
//#undef SIMPLE


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

    Vector
    chunkedParallelMatrixVectorMultiplication(SparseMatrix2D const & m, Vector const & x) {
        using size_type = IMatrix2D::size_type;
        Vector result{x.size()};
        size_type numberOfProcessors = std::thread::hardware_concurrency();
        IMatrix2D::size_type chunk_size = x.size() / numberOfProcessors;
        auto size = common_NS::getAdjustedSize(x.size(), numberOfProcessors);
        concurrency::parallel_for(size_type{0}, size, chunk_size, [&m, &x, &result, numberOfProcessors](size_type row) {
            size_type start_row, end_size;
            std::tie(start_row, end_size) = common_NS::getChunkStartEndIndex(x.size(), size_type{numberOfProcessors}, row);
            for (size_type i = start_row; i < end_size; ++i) {
                double result_row = LinAlg_NS::helper::matrix_vector_mul<Vector>(m, x, i);
                result(i) = result_row;
            }
        }, concurrency::static_partitioner());
        return result;
    }
    
    SparseMatrix2D
    nonChunkedParallelMatrixMatrixMultiplication(SparseMatrix2D const & lhs, SparseMatrix2D const & rhs) {
        common_NS::reporting::checkConditional(lhs.cols() == rhs.rows(), "helper::matrixMul: Matrices incompatible");
        auto nrows = lhs.rows();
        auto ncols = rhs.cols();
        SparseMatrix2D result{nrows, ncols};
        using size_type = IMatrix2D::size_type;

        // each chunk must only access its own private memory as
        // SparseMatrix2D as the call to SparseMatrix2D::operator(row, column)
        // is not thread safe!
        using TupleType_t = std::tuple<size_type, size_type, double>;
        std::vector<std::forward_list<TupleType_t>> chunkPrivateMemory{nrows};

        concurrency::parallel_for(size_type{0}, nrows, [&lhs, &rhs, &chunkPrivateMemory, ncols](size_type row) {
            auto value = 0.0;
            ConstRowColumnIterator<SparseMatrix2D> columnRowIterator = MatrixIterators::getConstRowColumnIterator(lhs, row);
            for (IMatrix2D::size_type column = 0; column < ncols; ++column) {
                ConstColumnRowIterator<SparseMatrix2D> rowColumnIterator = MatrixIterators::getConstColumnRowIterator(rhs, column);
                ConstColumnIterator<SparseMatrix2D> columnIterator = *columnRowIterator;
                ConstRowIterator<SparseMatrix2D> rowIterator = *rowColumnIterator;

                // 1st element in row'th row of lhs: lhs(row, columnIterator.column())
                while (columnIterator && columnIterator.column() < lhs.cols()) {
                    while (rowIterator && rowIterator.row() < columnIterator.column()) {
                        ++rowIterator;
                    }
                    if (!rowIterator)
                        break;
                    if (rowIterator.row() == columnIterator.column())
                        value += lhs(row, rowIterator.row()) * rhs(rowIterator.row(), column);
                    ++columnIterator;
                }
                if (value) {
//                        result(row, column) = value;
                    chunkPrivateMemory[row].push_front(std::make_tuple(row, column, value));
                    value = 0.0;
                }
            }
        });
        std::for_each(std::cbegin(chunkPrivateMemory), std::cend(chunkPrivateMemory), [&result](std::forward_list<TupleType_t> const & chunkItem) {
            std::for_each(std::cbegin(chunkItem), std::cend(chunkItem), [&result](std::tuple<size_type, size_type, double> const & item) {
                auto row = std::get<0>(item);
                auto column = std::get<1>(item);
                auto value = std::get<2>(item);
                result(row, column) = value;
            });

        });
        result.finalize();
        return result;
    }

    SparseMatrix2D
    chunkedParallelMatrixMatrixMultiplication(SparseMatrix2D const & lhs, SparseMatrix2D const & rhs) {
        common_NS::reporting::checkConditional(lhs.cols() == rhs.rows(), "helper::matrixMul: Matrices incompatible");
        auto nrows = lhs.rows();
        auto ncols = rhs.cols();
        SparseMatrix2D result{nrows, ncols};

        using size_type = IMatrix2D::size_type;
        size_type numberOfProcessors = std::thread::hardware_concurrency();
        if (nrows < numberOfProcessors)
            numberOfProcessors = nrows;
        IMatrix2D::size_type chunk_size = nrows / numberOfProcessors;
        auto size = common_NS::getAdjustedSize(nrows, numberOfProcessors);

        // each chunk must only access its own private memory as
        // SparseMatrix2D as the call to SparseMatrix2D::operator(row, column)
        // is not thread safe!
        using TupleType_t = std::tuple<size_type, size_type, double>;
        std::vector<std::forward_list<TupleType_t>> chunkPrivateMemory{numberOfProcessors};

        concurrency::parallel_for(size_type{0}, size, chunk_size, [&lhs, &rhs, &chunkPrivateMemory, nrows, ncols, numberOfProcessors, chunk_size](size_type row_index) {
            size_type start_index, end_size;
            std::tie(start_index, end_size) = common_NS::getChunkStartEndIndex(nrows, size_type{numberOfProcessors}, row_index);
            for (size_type row = start_index; row < end_size; ++row) {
                auto value = 0.0;
                ConstRowColumnIterator<SparseMatrix2D> columnRowIterator = MatrixIterators::getConstRowColumnIterator(lhs, row);
                for (IMatrix2D::size_type column = 0; column < ncols; ++column) {
                    ConstColumnRowIterator<SparseMatrix2D> rowColumnIterator = MatrixIterators::getConstColumnRowIterator(rhs, column);
                    ConstColumnIterator<SparseMatrix2D> columnIterator = *columnRowIterator;
                    ConstRowIterator<SparseMatrix2D> rowIterator = *rowColumnIterator;

                    // 1st element in row'th row of lhs: lhs(row, columnIterator.column())
                    while (columnIterator && columnIterator.column() < lhs.cols()) {
                        while (rowIterator && rowIterator.row() < columnIterator.column()) {
                            ++rowIterator;
                        }
                        if (!rowIterator)
                            break;
                        if (rowIterator.row() == columnIterator.column())
                            value += lhs(row, rowIterator.row()) * rhs(rowIterator.row(), column);
                        ++columnIterator;
                    }
                    if (value) {
//                        result(row, column) = value;
                        auto chunk_index = row_index / chunk_size;
                        chunkPrivateMemory[chunk_index].push_front(std::make_tuple(row, column, value));
                        value = 0.0;
                    }
                }
            }
        });//, concurrency::static_partitioner);
        std::for_each(std::cbegin(chunkPrivateMemory), std::cend(chunkPrivateMemory), [&result](std::forward_list<TupleType_t> const & chunkItem) {
            std::for_each(std::cbegin(chunkItem), std::cend(chunkItem), [&result](std::tuple<size_type, size_type, double> const & item) {
                auto row = std::get<0>(item);
                auto column = std::get<1>(item);
                auto value = std::get<2>(item);
                result(row, column) = value;
            });
        });
        result.finalize();
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
ParallelLinAlgOperationsTest::testChunkGenerationAlgorithmUneven() {
    using size_type = IMatrix2D::size_type;
    size_type numberOfProcessors = std::thread::hardware_concurrency();
    size_type total_size = 19;
    IMatrix2D::size_type chunk_size = total_size / numberOfProcessors;
    auto adjusted_size = common_NS::getAdjustedSize(total_size, numberOfProcessors);
    
    size_type expected = 19 / numberOfProcessors * numberOfProcessors;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("chunk adjusted size mismatch", expected, adjusted_size);

    /* 19 / 8 = 2 mod 3, so
     * 8 chunks with indices
     * 1. 0, 1, 2
     * 2. 3, 4, 5
     * 3. 6, 7, 8
     * 4. 9, 10
     * 5. 11, 12
     * 6. 13, 14
     * 7. 15, 16
     * 8. 17, 18
     */

    // chunk 2
    size_type start_index, end_size;
    std::tie(start_index, end_size) = common_NS::getChunkStartEndIndex(total_size, size_type{numberOfProcessors}, size_type{chunk_size});
    expected = 3;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("chunk start index mismatch", expected, start_index);
    expected = 6;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("chunk end index mismatch", expected, end_size);

    // chunk 4
    std::tie(start_index, end_size) = common_NS::getChunkStartEndIndex(total_size, size_type{numberOfProcessors}, size_type{3 * chunk_size});
    expected = 9;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("chunk start index mismatch", expected, start_index);
    expected = 11;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("chunk end index mismatch", expected, end_size);

    // chunk 8
    std::tie(start_index, end_size) = common_NS::getChunkStartEndIndex(total_size, size_type{ numberOfProcessors }, size_type{ 7 * chunk_size });
    expected = 17;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("chunk start index mismatch", expected, start_index);
    expected = 19;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("chunk end index mismatch", expected, end_size);
}

void
ParallelLinAlgOperationsTest::testChunkGenerationAlgorithmEven() {
    using size_type = IMatrix2D::size_type;
    size_type numberOfProcessors = 7;
    size_type total_size = 21;
    IMatrix2D::size_type chunk_size = total_size / numberOfProcessors;
    auto adjusted_size = common_NS::getAdjustedSize(total_size, numberOfProcessors);
    
    size_type expected = 21 / numberOfProcessors * numberOfProcessors;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("chunk adjusted size mismatch", expected, adjusted_size);

    /* 21 / 7 = 3 mod 0, so
     * 8 chunks with indices
     * 1. 0, 1, 2
     * 2. 3, 4, 5
     * 3. 6, 7, 8
     * 4. 9, 10, 11
     * 5. 12, 13, 14
     * 5. 15, 16, 17
     * 6. 18, 19, 20
     */

    // chunk 2
    size_type start_index, end_size;
    std::tie(start_index, end_size) = common_NS::getChunkStartEndIndex(total_size, size_type{numberOfProcessors}, size_type{chunk_size});
    expected = 3;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("chunk start index mismatch", expected, start_index);
    expected = 6;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("chunk end index mismatch", expected, end_size);

    // chunk 4
    std::tie(start_index, end_size) = common_NS::getChunkStartEndIndex(total_size, size_type{numberOfProcessors}, size_type{3 * chunk_size});
    expected = 9;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("chunk start index mismatch", expected, start_index);
    expected = 12;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("chunk end index mismatch", expected, end_size);

    // chunk 7
    std::tie(start_index, end_size) = common_NS::getChunkStartEndIndex(total_size, size_type{ numberOfProcessors }, size_type{ 6 * chunk_size });
    expected = 18;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("chunk start index mismatch", expected, start_index);
    expected = 21;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("chunk end index mismatch", expected, end_size);
}

void
ParallelLinAlgOperationsTest::testNonChunkedParallelMatrixVectorMultiplication() {
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
ParallelLinAlgOperationsTest::testChunkedParallelMatrixVectorMultiplication() {
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
ParallelLinAlgOperationsTest::testNonChunkedParallelDotProduct() {
    IMatrix2D::size_type dim = 999130;
    Vector v1 = createVectorOfSize(dim);
    Vector v2 = createVectorOfSize(dim);

    double serial_result;
    {
        HighResTimer t;
        serial_result = LinAlg_NS::VectorMath::dotProductSerial(v1, v2);
    }

    double parallel_result;
    {
        HighResTimer t;
        parallel_result = LinAlg_NS::VectorMath::nonChunkedParallelDotProduct(v1, v2);
    }
    // The large tolerance is due to the fact that associativity does not
    // hold when evaluated in parallel.
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("serial & parallel dot product mismatch", serial_result, parallel_result, 1E-7);
}

void
ParallelLinAlgOperationsTest::testChunkedParallelDotProduct() {
    IMatrix2D::size_type dim = 9199130;
    Vector v1 = createVectorOfSize(dim);
    Vector v2 = createVectorOfSize(dim);

    double serial_result;
    {
        HighResTimer t;
        serial_result = LinAlg_NS::VectorMath::dotProductSerial(v1, v2);
    }

    double parallel_result;
    {
        HighResTimer t;
        parallel_result = LinAlg_NS::VectorMath::chunkedParallelDotProduct(v1, v2);
    }
    // The large tolerance is due to the fact that associativity does not
    // hold when evaluated in parallel.
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("serial & parallel dot product mismatch", serial_result, parallel_result, 1E-7);
}

void
ParallelLinAlgOperationsTest::testNonChunkedParallelMatrixProduct() {
    MatrixStencil<PeriodicBoundaryConditionPolicy> stencil;
    stencil <<
         2, -1,  9,  2,  1,
        -1,  4, -1, -6, -3,
         7, -1,  3, -7, -8,
         3,  5, -8, -9, -3,
         0,  1, -2,  7,  2;
    
    // 25x25 square matrix
    SparseMatrix2D const & m = stencil.generateMatrix(5 * 5);

    Vector v{m.cols()};
    std::iota(std::begin(v), std::end(v), 1);

    SparseMatrix2D serial_result;
    {
        HighResTimer t;
        serial_result = helper::matrixMul(m, m);
    }
//    serial_result.print();

    SparseMatrix2D parallel_result;
    {
        HighResTimer t;
        parallel_result = nonChunkedParallelMatrixMatrixMultiplication(m, m);
    }
//    parallel_result.print();

    Vector result1;
    result1 = serial_result * v;

    Vector result2;
    result2 = parallel_result * v;

    CPPUNIT_ASSERT_MESSAGE("matrix-matrix multiplication mismatch", SparseLinearSolverUtil::isVectorEqual(result1, result2, 1E-12));
    if (SparseLinearSolverUtil::isVectorEqual(result1, result2, 1E-12) == false)
        __debugbreak();
}

void
ParallelLinAlgOperationsTest::testChunkedParallelMatrixProduct() {
    MatrixStencil<PeriodicBoundaryConditionPolicy> stencil;
    stencil <<
         2, -1,  9,  2,  1,
        -1,  4, -1, -6, -3,
         7, -1,  3, -7, -8,
         3,  5, -8, -9, -3,
         0,  1, -2,  7,  2;

    // 25x25 square matrix
    SparseMatrix2D const & m = stencil.generateMatrix(5 * 5);

    Vector v{ m.cols() };
    std::iota(std::begin(v), std::end(v), 1);

    SparseMatrix2D serial_result;
    {
        HighResTimer t;
        serial_result = helper::matrixMul(m, m);
    }
//    serial_result.print();

    SparseMatrix2D parallel_result;
    {
        HighResTimer t;
        parallel_result = chunkedParallelMatrixMatrixMultiplication(m, m);
    }
//    parallel_result.print();

    Vector result1;
    result1 = serial_result * v;

    Vector result2;
    result2 = parallel_result * v;

//     if (SparseLinearSolverUtil::isVectorEqual(result1, result2, 1E-12) == false)
//         __debugbreak();
    CPPUNIT_ASSERT_MESSAGE("matrix-matrix multiplication mismatch", SparseLinearSolverUtil::isVectorEqual(result1, result2, 1E-12));
}

void
ParallelLinAlgOperationsTest::testNonChunkedParallelNonSquareMatrixProduct() {
    MatrixStencil<PeriodicBoundaryConditionPolicy> stencil;
    stencil <<
         2, -1,  9,  2,  1,
        -1,  4, -1, -6, -3,
         7, -1,  3, -7, -8,
         3,  5, -8, -9, -3,
         0,  1, -2,  7,  2;

    // 25x25 square matrix
    SparseMatrix2D const & m = stencil.generateMatrix(5 * 5);


    // generate a non-square 7x3 matrix form m
    SparseMatrix2D m1{ 7, 3 };
    for (auto row = 7; row < 7 + 7; ++row) {
        for (auto column = 7; column < 7 + 3; ++column) {
            m1(row - 7, column - 7) = m(row, column);
        }
    }
    m1.finalize();
//    m1.print();

    // generate a non-square 3x2 matrix form m
    SparseMatrix2D m2{ 3, 2 };
    for (auto row = 10; row < 10 + 3; ++row) {
        for (auto column = 12; column < 12 + 2; ++column) {
            m2(row - 10, column - 12) = m(row, column);
        }
    }
    m2.finalize();
//    m2.print();


    Vector v{ m2.cols() };
    std::iota(std::begin(v), std::end(v), 1);

    SparseMatrix2D serial_result;
    {
        HighResTimer t;
        serial_result = helper::matrixMul(m1, m2);
    }
//    serial_result.print();

    SparseMatrix2D parallel_result;
    {
        HighResTimer t;
       parallel_result = nonChunkedParallelMatrixMatrixMultiplication(m1, m2);
    }
//    parallel_result.print();

    Vector result1;
    result1 = serial_result * v;

    Vector result2;
    result2 = parallel_result * v;

    if (SparseLinearSolverUtil::isVectorEqual(result1, result2, 1E-12) == false)
        __debugbreak();
    CPPUNIT_ASSERT_MESSAGE("matrix-matrix multiplication mismatch", SparseLinearSolverUtil::isVectorEqual(result1, result2, 1E-12));
}

void
ParallelLinAlgOperationsTest::testChunkedParallelNonSquareMatrixProduct() {
    MatrixStencil<PeriodicBoundaryConditionPolicy> stencil;
    stencil <<
         2, -1,  9,  2,  1,
        -1,  4, -1, -6, -3,
         7, -1,  3, -7, -8,
         3,  5, -8, -9, -3,
         0,  1, -2,  7,  2;

    // 25x25 square matrix
    SparseMatrix2D const & m = stencil.generateMatrix(5 * 5);


    // generate a non-square 7x3 matrix form m
    SparseMatrix2D m1{7, 3};
    for (auto row = 7; row < 7 + 7; ++row) {
        for (auto column = 7; column < 7 + 3; ++column) {
            m1(row - 7, column - 7) = m(row, column);
        }
    }
    m1.finalize();
//    m1.print();

    // generate a non-square 3x2 matrix form m
    SparseMatrix2D m2{3, 2};
    for (auto row = 10; row < 10 + 3; ++row) {
        for (auto column = 12; column < 12 + 2; ++column) {
            m2(row - 10, column - 12) = m(row, column);
        }
    }
    m2.finalize();
//    m2.print();


    Vector v{m2.cols()};
    std::iota(std::begin(v), std::end(v), 1);

    SparseMatrix2D serial_result;
    {
        HighResTimer t;
        serial_result = helper::matrixMul(m1, m2);
    }
//    serial_result.print();

    SparseMatrix2D parallel_result;
    {
        HighResTimer t;
        parallel_result = chunkedParallelMatrixMatrixMultiplication(m1, m2);
    }
//    parallel_result.print();

    Vector result1;
    result1 = serial_result * v;

    Vector result2;
    result2 = parallel_result * v;

    if (SparseLinearSolverUtil::isVectorEqual(result1, result2, 1E-12) == false)
        __debugbreak();
    CPPUNIT_ASSERT_MESSAGE("matrix-matrix multiplication mismatch", SparseLinearSolverUtil::isVectorEqual(result1, result2, 1E-12));
}

void
ParallelLinAlgOperationsTest::testNonChunkedMatrixIsSymmetric() {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil <<
         0,  0, -1,  0,  0,
         0,  0, -1,  0,  0,
        -1, -1, 20, -1, -1,
         0,  0, -1,  0,  0,
         0,  0, -1,  0,  0;

    // 25x25 square matrix
    SparseMatrix2D const & m = stencil.generateMatrix(5 * 5);

    // in order to modify the matrix to make it asymmetric, we have to create a new one
    SparseMatrix2D m_asymmetric{m.rows(), m.cols()};
    for (SparseMatrix2D::size_type row = 0; row < m.rows(); ++row) {
        for (SparseMatrix2D::size_type column = 0; column < m.cols(); ++column) {
            double value = m(row, column);
            if (value)
                m_asymmetric(row, column) = value;
        }
    }
    m_asymmetric(m.rows() - 1, m.cols() - 2) = 0.1;
    m_asymmetric.finalize();
//    m_asymmetric.print();

    bool serial_result;
    {
        HighResTimer t;
        serial_result = helper::isSymmetric(m_asymmetric);
    }

    bool parallel_result;
    {
        HighResTimer t;
        parallel_result = LinAlg_NS::helper::matrixIsSymmetricParallelNonChunked(m_asymmetric);
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("symmetry mismatch between serial and parallel version", serial_result, parallel_result);
}

void
ParallelLinAlgOperationsTest::testChunkedMatrixIsSymmetric() {
    MatrixStencil<DirichletBoundaryConditionPolicy> stencil;
    stencil <<
        0, 0, -1, 0, 0,
        0, 0, -1, 0, 0,
        -1, -1, 20, -1, -1,
        0, 0, -1, 0, 0,
        0, 0, -1, 0, 0;

    // 25x25 square matrix
    SparseMatrix2D const & m = stencil.generateMatrix(5 * 5);

    // in order to modify the matrix to make it asymmetric, we have to create a new one
    SparseMatrix2D m_asymmetric{ m.rows(), m.cols() };
    for (SparseMatrix2D::size_type row = 0; row < m.rows(); ++row) {
        for (SparseMatrix2D::size_type column = 0; column < m.cols(); ++column) {
            double value = m(row, column);
            if (value)
                m_asymmetric(row, column) = value;
        }
    }
    m_asymmetric(m.rows() - 1, m.cols() - 2) = 0.1;
    m_asymmetric.finalize();
//    m_asymmetric.print();

    bool serial_result;
    {
        HighResTimer t;
        serial_result = helper::isSymmetric(m_asymmetric);
    }

    bool parallel_result;
    {
        HighResTimer t;
        parallel_result = LinAlg_NS::helper::matrixIsSymmetricParallelChunked(m_asymmetric);
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("symmetry mismatch between serial and parallel version", serial_result, parallel_result);
}
