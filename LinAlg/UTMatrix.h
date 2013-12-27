/*
 * Name  : UTMatrix
 * Path  : 
 * Use   : Square upper triangular matrix,
 *         minimal dense square upper triangular matrix for use in
 *         GMRES implementation.  Matrix index scheme is:
 *
 *	       [0 1 3 6 ... ] 
 *	       [  2 4 7 ... ] 
 *	       [    5 8 ... ] 
 *	       [      9 ... ] 
 *	       [      . ... ] 
 *	       [      . ... ] 
 *	       [      . ... ]
 *
 *         The index of column j is
 *         j * (j + 1) / 2.
 *         Note: A triangular matrix is by definition square!
 *
 *         Storage format: column-major
 * Author: Sven Schmidt
 * Date  : 04/07/2013
 */
#pragma once

#include "DeclSpec.h"

#include "IMatrix2D.h"


#pragma warning(disable:4251)
#pragma warning(disable:4275)


namespace LinAlg_NS {

    class LINALG_DECL_SYMBOLS UTMatrix : public IMatrix2D {
    public:
        UTMatrix();
        UTMatrix(size_type ncols);
        UTMatrix(UTMatrix const & in);

        UTMatrix & operator=(UTMatrix const & in);

        // enable move semantics
        UTMatrix(UTMatrix && in);
        UTMatrix & operator=(UTMatrix && in);

        // FROM IMatrix2D
        size_type rows() const;
        size_type cols() const;
        double    operator()(size_type row, size_type col) const;
        double &  operator()(size_type row, size_type col);

        // Local methods
        void print() const;

    private:
        // to provide exception-safe copy-assignment
        void swap(UTMatrix const & in);

    private:
        // Number of columns
        size_type           ncols_;

        // data
        std::vector<double> data_;
    };

} // namespace LinAlg_NS

#pragma warning(default:4275)
#pragma warning(default:4251)
