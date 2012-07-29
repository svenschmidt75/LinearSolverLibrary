/*
 * Name  : Matrix2D
 * Path  : IMatrix2D
 * Use   : Implements a dense 2D matrix representation
 * Author: Sven Schmidt
 * Date  : 12/3/2011
 */
#pragma once

#include "DeclSpec.h"

#include "IMatrix2D.h"

#include <vector>

#include <boost/cstdint.hpp>

namespace LinAlg_NS {

    class Vector;


    class Matrix2D : public IMatrix2D {
    public:
        // To be used as base class
        Matrix2D(size_type rows, size_type cols);
        Matrix2D(Matrix2D const & in);
        Matrix2D & operator=(Matrix2D const & in);

        // enables move semantics
        Matrix2D(Matrix2D && in);
        Matrix2D & operator=(Matrix2D && in);

        // FROM IMatrix2D
        size_type getRows() const;
        size_type getCols() const;
        double    operator()(size_type row, size_type col) const;
        double&   operator()(size_type row, size_type col);

        // solve A x = b
        void            solve(Vector const & b, Vector & x) const;

        // Local Methods
        void           print() const;

        // Static Methods
        static Matrix2D identity(size_type n);

    private:
        // to provide exception-safe copy-assignment
        void swap(Matrix2D const & in);

    private:
        std::vector<double> data_;

        // rows and cols
        size_type rows_;
        size_type cols_;
    };

} // namespace LinAlg_NS
