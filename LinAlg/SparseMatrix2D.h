/*
 * Name  : SparseMatrix2D
 * Path  : IMatrix2D
 * Use   : Implements a sparse 2D matrix representation.
 *         The matrix elements are first inserted and once finished,
 *         the internal matrix representation is converted into the
 *         compressed storage format.
 * Author: Sven Schmidt
 * Date  : 12/26/2011
 */
#pragma once

#include "DeclSpec.h"

#include "IMatrix2D.h"

#include <map>
#include <vector>

#include <boost/cstdint.hpp>


#pragma warning(disable:4251)
#pragma warning(disable:4275)


namespace LinAlg_NS {

    class Vector;


    class DECL_SYMBOLS SparseMatrix2D : public IMatrix2D {

        friend struct LinearSolver;

    public:
        SparseMatrix2D(size_type ncols);
        SparseMatrix2D(SparseMatrix2D const & in);
        SparseMatrix2D & operator=(SparseMatrix2D const & in);

        // enable move semantics
        SparseMatrix2D(SparseMatrix2D const && in);
        SparseMatrix2D & operator=(SparseMatrix2D const && in);

        // FROM IMatrix2D
        size_type getRows() const;
        size_type getCols() const;
        double    operator()(size_type row, size_type col) const;
        double &  operator()(size_type row, size_type col);
        void      solve(Vector const & b, Vector & x) const;

        // Local methods
        void finalize() const;
        void print() const;

    private:
        typedef std::map<size_type, double> Col_t;
        typedef std::map<size_type, Col_t> Row_t;

    private:
        // to provide exception-safe copy-assignment
        void swap(SparseMatrix2D const & in);

    private:
        // Number of columns
        size_type     ncols_;

        // row-major format
        mutable Row_t data_;

        // Check whether the matrix has already been converted to the
        // compresses row storage format
        mutable bool  finalized_;

        /* Compressed row storage format
         * 
         * elements_ contains the non-zero elements in sequential order.
         * columns_ contains the column for the element.
         * nelements_: Contains the increasing number of elements per row.
         * 
         * The number of rows can be determined from nelements_: 
         * nelements_.size() == #rows + 1
         */
        mutable std::vector<double>    elements_;
        mutable std::vector<size_type> columns_;
        mutable std::vector<size_type> nelements_;
    };

} // namespace LinAlg_NS

#pragma warning(default:4275)
#pragma warning(default:4251)
