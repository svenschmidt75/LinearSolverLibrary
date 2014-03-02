#include "pch.h"

#include "helper.h"
#include "Vector.h"


namespace LinAlg_NS {

SparseMatrix2D
helper::transpose(SparseMatrix2D const & m) {
    /* Transpose a sparse matrix. My first idea was to write a wrapper that behaves like the transpose,
     * but when multiplying this wrapper with a vector, we end up doing a dense matrix multiplication.
     * Hence this explicit construction.
     */
#ifdef PARALLEL
    return transposeParallelNonChunked(m);
//    return transposeParallelChunked(m);
#else
    return transposeSerial(m);
#endif
}

SparseMatrix2D
helper::transposeSerial(SparseMatrix2D const & m) {
    /* Transpose a sparse matrix. My first idea was to write a wrapper that behaves like the transpose,
        * but when multiplying this wrapper with a vector, we end up doing a dense matrix multiplication.
        * Hence this explicit construction.
        */
    auto nrows = m.rows();
    auto ncols = m.cols();
    SparseMatrix2D transpose{ncols, nrows};
    for (IMatrix2D::size_type row{0}; row < nrows; ++row) {
        ConstRowColumnIterator<SparseMatrix2D> columnRowIterator = MatrixIterators::getConstRowColumnIterator(m, row);
        ConstColumnIterator<SparseMatrix2D> columnIterator = *columnRowIterator;
        while (columnIterator) {
            auto column = columnIterator.column();
            double value = m(row, column);
            if (value)
                transpose(column, row) = value;
            ++columnIterator;
        }
    }
    transpose.finalize();
    return transpose;
}

SparseMatrix2D
helper::transposeParallelNonChunked(SparseMatrix2D const & m) {
    auto nrows = m.rows();
    auto ncols = m.cols();
    SparseMatrix2D m_transpose{ncols, nrows};
    using size_type = IMatrix2D::size_type;

    // each chunk must only access its own private memory as
    // SparseMatrix2D as the call to SparseMatrix2D::operator(row, column)
    // is not thread safe!
    using TupleType_t = std::tuple<size_type, size_type, double>;
    std::vector<std::vector<TupleType_t>> chunkPrivateMemory{nrows};
    for (size_type row{0}; row < nrows; ++row) {
        ConstRowColumnIterator<SparseMatrix2D> columnRowIterator = MatrixIterators::getConstRowColumnIterator(m, row);
        chunkPrivateMemory[row].reserve(columnRowIterator.numberOfNonZeroMatrixElements());
    }
    concurrency::parallel_for(size_type{0}, nrows, [&m, &chunkPrivateMemory, ncols](size_type row) {
        ConstRowColumnIterator<SparseMatrix2D> columnRowIterator = MatrixIterators::getConstRowColumnIterator(m, row);
        ConstColumnIterator<SparseMatrix2D> columnIterator = *columnRowIterator;
        while (columnIterator) {
            auto column = columnIterator.column();
            double value = m(row, column);
            if (value)
                chunkPrivateMemory[row].push_back(std::make_tuple(row, column, value));
            ++columnIterator;
        }
    });
    for (size_type row{0}; row < nrows; ++row) {
        std::vector<TupleType_t> const & items = chunkPrivateMemory[row];
        for (size_type item_position{0}; item_position < items.size(); ++item_position) {
            TupleType_t const & item = items[item_position];
            auto row = std::get<0>(item);
            auto column = std::get<1>(item);
            auto value = std::get<2>(item);
            m_transpose(column, row) = value;
        }
    }
    m_transpose.finalize();
    return m_transpose;
}

SparseMatrix2D
helper::transposeParallelChunked(SparseMatrix2D const & m) {
    auto nrows = m.rows();
    auto ncols = m.cols();
    SparseMatrix2D m_transpose{ncols, nrows};

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
    std::vector<std::vector<TupleType_t>> chunkPrivateMemory{nrows};
    for (size_type row{0}; row < nrows; ++row) {
        ConstRowColumnIterator<SparseMatrix2D> columnRowIterator = MatrixIterators::getConstRowColumnIterator(m, row);
        chunkPrivateMemory[row].reserve(columnRowIterator.numberOfNonZeroMatrixElements());
    }
    concurrency::parallel_for(size_type{0}, size, chunk_size, [&m, &chunkPrivateMemory, nrows, ncols, numberOfProcessors, chunk_size](size_type row_index) {
        size_type start_index, end_size;
        std::tie(start_index, end_size) = common_NS::getChunkStartEndIndex(nrows, size_type{numberOfProcessors}, row_index);
        for (size_type row = start_index; row < end_size; ++row) {
            ConstRowColumnIterator<SparseMatrix2D> columnRowIterator = MatrixIterators::getConstRowColumnIterator(m, row);
            ConstColumnIterator<SparseMatrix2D> columnIterator = *columnRowIterator;
            while (columnIterator) {
                auto column = columnIterator.column();
                double value = m(row, column);
                if (value)
                    chunkPrivateMemory[row].push_back(std::make_tuple(row, column, value));
                ++columnIterator;
            }
        }
    });//, concurrency::static_partitioner);
    for (size_type row{0}; row < nrows; ++row) {
        std::vector<TupleType_t> const & items = chunkPrivateMemory[row];
        for (size_type item_position{0}; item_position < items.size(); ++item_position) {
            TupleType_t const & item = items[item_position];
            auto row = std::get<0>(item);
            auto column = std::get<1>(item);
            auto value = std::get<2>(item);
            m_transpose(column, row) = value;
        }
    }
    m_transpose.finalize();
    return m_transpose;
}

template<typename MATRIX_EXPR>
double
helper::get_value(MATRIX_EXPR const & m, IMatrix2D::size_type row, IMatrix2D::size_type col) {
    double value = m(row, col);
    return value;
}

bool
helper::isSymmetric(SparseMatrix2D const & m) {
    if (m.rows() != m.cols())
        return false;
#ifdef PARALLEL
    return matrixIsSymmetricParallelNonChunked(m);
//    return matrixIsSymmetricParallelChunked(m);
#else
    return isSymmetricSerial(m);
#endif
}

bool
helper::isSymmetricSerial(SparseMatrix2D const & m) {
    if (m.rows() != m.cols())
        return false;
    /* Access pattern of algorithm:
     * 
     *       |             ...
     *       | m(i,0) ... m(i,i) m{i,i+1} ... m(i,cols)|
     *       |             ...
     *       | ...        m(j,k)    ...
     *       | ...        m(j+1,k)  ...
     *       | ...         ...      ...
     *       | ...        m(rows,k) ...
     * 
     * For each row i, all elements a(i,k) with k = i + 1 ... cols
     * (i.e. to the right of the diagonal element m(i,i)) are compared
     * to the elements in column i, a(k,i).
     * Thus, the access pattern is like this:
     * 
     * 
     * 
     * 
     * 
     *              ith column
     *                 \/
     *      ith row: >  ************************
     *                  * ...
     *                  * ...
     *                  * ...
     *                  * ...
     *                  * ...
     *                  * ...
     *                  * ...
     */
    using size_type = IMatrix2D::size_type;
    static double const tol = 1E-10;

    // m.rows() + 1, because the last for the last row, there is
    // only the diagonal element a(m.rows() - 1, m.rows() - 1),
    // so nothink to check.
    for (size_type row{0}; row < m.rows() - 1; ++row) {
        for (size_type col{row + 1}; col < m.cols(); ++col) {
            double a_ij = m(row, col);
            double a_ji = m(col, row);
            double delta = std::fabs(a_ij - a_ji);
            if (delta > tol)
                return false;
        }
    }
    return true;
}

bool
helper::matrixIsSymmetricParallelNonChunked(SparseMatrix2D const & m) {
    if (m.rows() != m.cols())
        return false;
    static double const tol = 1E-10;
    using size_type = IMatrix2D::size_type;
    bool is_symmetric = true;
    concurrency::cancellation_token_source cancellation_token;
    concurrency::run_with_cancellation_token([&m, &cancellation_token, &is_symmetric]() {
        concurrency::parallel_for(size_type{0}, m.rows() - 1, [&m, &cancellation_token, &is_symmetric](size_type row) {
            for (size_type col{row + 1}; col < m.cols(); ++col) {
                double a_ij = m(row, col);
                double a_ji = m(col, row);
                double delta = std::fabs(a_ij - a_ji);
                if (delta > tol) {
                    is_symmetric = false;
                    cancellation_token.cancel();
                }
            }
        });
    }, cancellation_token.get_token());
    return is_symmetric;
}
    
bool
helper::matrixIsSymmetricParallelChunked(SparseMatrix2D const & m) {
    /* The performance here sucks compared to 'matrixIsSymmetricParallelNonChunked' above.
        * Not sure why, but threads with low row index work harder than those with
        * high row number due to the access pattern. For an explanation of the access
        * patterns, see helper::isSymmetric.
        * 
        * According to the concurrency visualizer, the slowdown is due to A LOT of
        * synchcronization, effectively killing concurrency. The reason is that I
        * only use 8 chunks (numberOfProcessors = std::thread::hardware_concurrency() = 8).
        * If I multiply this by a larger number, we get close to the performance of
        * 'matrixIsSymmetricParallelNonChunked'. Interesting...
        * 
        * Comparison:
        * 
        *  numberOfProcessors |  serial  | non chunked parallel | chunked parallel |
        *  -------------------------------------------------------------------------
        *         8           |  4.05258 | 0.0573509            | 1.24664          |
        *         8 * 2       |          | 0.146136             | 0.851729         |
        *         8 * 8       |          | 0.0980501            | 0.23172          |
        *         8 * 1024    |          | 0.040534             | 0.087157         |
        *         8 * 2048    |          | 0.09845              | 0.0565017        |
        * 
        * It follows that even chunking only makes sense when all chunks have an even
        * load. Otherwise, scheduling based on load by the CRT is much more beneficial.
        */
    if (m.rows() != m.cols())
        return false;
    static double const tol = 1E-10;
    using size_type = IMatrix2D::size_type;
    bool is_symmetric = true;
    size_type ncols = m.cols();
    size_type nrows = m.rows();
    concurrency::cancellation_token_source cancellation_token;
    size_type numberOfProcessors = std::thread::hardware_concurrency();
    if (nrows < numberOfProcessors)
        numberOfProcessors = nrows;
    size_type chunk_size = nrows / numberOfProcessors;
#if _DEBUG
    common_NS::reporting::checkConditional(chunk_size, "chunk_size cannot be null");
#endif
    auto size = common_NS::getAdjustedSize(nrows, numberOfProcessors);
    concurrency::run_with_cancellation_token([&m, &cancellation_token, &is_symmetric, ncols, nrows, size, chunk_size, numberOfProcessors]() {
        concurrency::parallel_for(size_type{0}, size, chunk_size, [&m, &cancellation_token, &is_symmetric, ncols, nrows, numberOfProcessors](size_type index) {
            size_type start_row, end_size;
            std::tie(start_row, end_size) = common_NS::getChunkStartEndIndex(nrows, size_type{numberOfProcessors}, index);
            for (size_type row{start_row}; row < end_size; ++row) {
                for (size_type col{row + 1}; col < ncols; ++col) {
                    double a_ij = m(row, col);
                    double a_ji = m(col, row);
                    double delta = std::fabs(a_ij - a_ji);
                    if (delta > tol) {
                        is_symmetric = false;
                        cancellation_token.cancel();
                    }
                }
            }
        });//, concurrency::static_partitioner());
    }, cancellation_token.get_token());
    return is_symmetric;
}

SparseMatrix2D
helper::matrixMul(SparseMatrix2D const & lhs, SparseMatrix2D const & rhs) {
    common_NS::reporting::checkConditional(lhs.cols() == rhs.rows(), "helper::matrixMul: Matrices incompatible");
    auto nrows = lhs.rows();
    auto ncols = rhs.cols();
    SparseMatrix2D tmp{nrows, ncols};
    for (IMatrix2D::size_type row = 0; row < nrows; ++row) {
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
                tmp(row, column) = value;
                value = 0.0;
            }
        }
    }
    tmp.finalize();
    return tmp;
}

Matrix2D
helper::SparseToDense(SparseMatrix2D const & sparse) {
    Matrix2D dense{sparse.rows(), sparse.cols()};
    for (IMatrix2D::size_type row{0}; row < sparse.rows(); ++row) {
        ConstRowColumnIterator<SparseMatrix2D> columnRowIterator = MatrixIterators::getConstRowColumnIterator(sparse, row);
        ConstColumnIterator<SparseMatrix2D> columnIterator = *columnRowIterator;
        while (columnIterator) {
            auto column = columnIterator.column();
            double value = sparse(row, column);
            if (value)
                dense(row, column) = value;
            ++columnIterator;
        }
    }
    return dense;
}

// create explicit template instantiations
template LINALG_DECL_SYMBOLS double helper::get_value(Matrix2D const & m, IMatrix2D::size_type row, IMatrix2D::size_type col);
template LINALG_DECL_SYMBOLS double helper::get_value(SparseMatrix2D const & m, IMatrix2D::size_type row, IMatrix2D::size_type col);

} // namespace LinAlg_NS
