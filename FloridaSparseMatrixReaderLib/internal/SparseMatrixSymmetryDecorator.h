/*
 * Name  : SparseMatrixSymmetryDecorator
 * Path  : 
 * Use   : The Matrix Market format is different depending on whether
           the matrix is symmetric or not. This decorator takes care
           of that automatically
 * Author: Sven Schmidt
 * Date  : 03/02/2013
 */
#pragma once


#include "LinAlg/IMatrix2D.h"
#include "LinAlg/SparseMatrix2D.h"
#include "LinAlg/Vector.h"

#include <boost/noncopyable.hpp>


namespace EntityReader_NS {

    class SparseMatrixSymmetryDecorator : LinAlg_NS::IMatrix2D, boost::noncopyable {
    public:
        SparseMatrixSymmetryDecorator(LinAlg_NS::SparseMatrix2D & in);

        // Public matrix interface
        size_type rows() const;
        size_type cols() const;
        double    operator()(size_type row, size_type col) const;
        double &  operator()(size_type row, size_type col);
        void      solve(LinAlg_NS::Vector const & b, LinAlg_NS::Vector & x);

        void      print() const;

    private:
        LinAlg_NS::SparseMatrix2D & m_;
    };

} // EntityReader_NS
