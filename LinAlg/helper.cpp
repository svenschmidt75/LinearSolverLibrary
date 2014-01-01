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
    SparseMatrix2D transposed(m.cols(), m.rows());
    for (SparseMatrix2D::size_type row = 0; row < m.rows(); ++row) {
        // Number of non-zero columns for this row
        IMatrix2D::size_type ncol = m.columns_offset_[row + 1] - m.columns_offset_[row];
        IMatrix2D::size_type offset = m.columns_offset_[row];

        // all non-zero columns
        for (int icol = 0; icol < ncol; ++icol) {
            IMatrix2D::size_type col = m.columns_[offset + icol];
            double a_ij = m.elements_[offset + icol];
            transposed(col, row) = a_ij;
        }
    }
    transposed.finalize();
    return transposed;
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

#if 1

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

#else

SparseMatrix2D
helper::matrixMul(SparseMatrix2D const & lhs, SparseMatrix2D const & rhs) {
    common_NS::reporting::checkConditional(lhs.cols() == rhs.rows(), "helper::matrixMul: Matrices incompatible");
    auto nrows = lhs.rows();
    auto ncols = rhs.cols();
    SparseMatrix2D tmp{nrows, ncols};

//     lhs.print();
//     rhs.print();


    for (IMatrix2D::size_type row = 0; row < nrows; ++row) {
        auto value = 0.0;
//        ConstRowColumnIterator<SparseMatrix2D> columnRowIterator = MatrixIterators::getConstRowColumnIterator(lhs, row);


        // get column indices for this row from iterator
        auto const & column_indices = lhs.getNonZeroColumnIndicesForRow(row);
        std::vector<IMatrix2D::size_type> non_zero_indices(lhs.cols());


        for (IMatrix2D::size_type column = 0; column < ncols; ++column) {
//            ConstColumnRowIterator<SparseMatrix2D> rowColumnIterator = MatrixIterators::getConstColumnRowIterator(rhs, column);

//            ConstColumnIterator<SparseMatrix2D> columnIterator = *columnRowIterator;
//            ConstRowIterator<SparseMatrix2D> rowIterator = *rowColumnIterator;


            // get row indices for this column from iterator
            auto const & row_indices = rhs.getNonZeroRowIndicesForColumn(column);

            auto end_it = std::set_intersection(std::cbegin(column_indices), std::cend(column_indices), std::cbegin(row_indices), std::cend(row_indices), std::begin(non_zero_indices));


            // 1st element in row'th row of lhs: lhs(row, columnIterator.column())
//             while (columnIterator && columnIterator.column() < lhs.cols()) {
//                 while (rowIterator && rowIterator.row() < columnIterator.column()) {
//                     ++rowIterator;
//                 }
//                 if (!rowIterator)
//                     break;
//                 if (rowIterator.row() == columnIterator.column())
//                     value += lhs(row, rowIterator.row()) * rhs(rowIterator.row(), column);
//                 ++columnIterator;
//             }

            double tmp_val = 0.0;
            auto it(std::cbegin(non_zero_indices));
            while (it != end_it) {
                size_type index = *it;
                ++it;
                tmp_val += lhs(row, index) * rhs(index, column);
            }
            value = tmp_val;

//             if (std::fabs(tmp_val - value) > 1E-10) {
//                 int a = 1;
//                 a++;
//             }

            if (value) {
                tmp(row, column) = value;
                value = 0.0;
            }
        }
    }
    tmp.finalize();
    return tmp;
}
#endif

// create explicit template instantiations
template LINALG_DECL_SYMBOLS double helper::get_value(Matrix2D const & m, IMatrix2D::size_type row, IMatrix2D::size_type col);
template LINALG_DECL_SYMBOLS double helper::get_value(SparseMatrix2D const & m, IMatrix2D::size_type row, IMatrix2D::size_type col);

} // namespace LinAlg_NS
