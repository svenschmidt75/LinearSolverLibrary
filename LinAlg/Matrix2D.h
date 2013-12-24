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

#include "common/reporting.h"

#include <vector>

#include <boost/cstdint.hpp>


#pragma warning(disable:4251)
#pragma warning(disable:4275)


namespace LinAlg_NS {

    class Vector;


    class LINALG_DECL_SYMBOLS Matrix2D : public IMatrix2D {
    public:
        Matrix2D(size_type rows, size_type cols);
        Matrix2D(Matrix2D const & in);
        Matrix2D & operator=(Matrix2D const & in);

        // enables move semantics
        Matrix2D(Matrix2D && in);
        Matrix2D & operator=(Matrix2D && in);

        // allow easy initialization
        Matrix2D & operator<<(double value);
        Matrix2D & operator,(double value);

        template<typename MATRIX_EXPR>
        Matrix2D & operator=(MATRIX_EXPR const & in) {
            static_assert(typename entity_traits<MATRIX_EXPR>::is_matrix_expression == true, "Matrix2D::operator=(): Input is not a matrix-like type");
            data_.resize(in.rows() * in.cols());
            for (IMatrix2D::size_type row = 0; row < in.rows(); ++row) {
                for (IMatrix2D::size_type col = 0; col < in.cols(); ++col) {
                    (*this)(row, col) = in(row, col);
                }
            }
            return *this;
        }

        // FROM IMatrix2D
        size_type rows() const override;
        size_type cols() const override;
        double    operator()(size_type row, size_type col) const override;
        double &  operator()(size_type row, size_type col) override;
        void      print() const override;

        // Local Methods

        // solve A x = b
        void            solve(Vector const & b, Vector & x) const;

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

#pragma warning(default:4275)
#pragma warning(default:4251)
