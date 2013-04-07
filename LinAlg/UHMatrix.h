/*
 * Name  : UHMatrix
 * Path  : 
 * Use   : Upper Hessenberg matrix
 * Author: Sven Schmidt
 * Date  : 04/07/2013
 */
#pragma once

#include "DeclSpec.h"

#include "IMatrix2D.h"

#include <boost/cstdint.hpp>

#include <vector>


#pragma warning(disable:4251)
#pragma warning(disable:4275)


namespace LinAlg_NS {

    class LINALG_DECL_SYMBOLS UHMatrix : public IMatrix2D {
    public:
        UHMatrix();
        UHMatrix(size_type ncols);
        UHMatrix(UHMatrix const & in);

        UHMatrix & operator=(UHMatrix const & in);

        // enable move semantics
        UHMatrix(UHMatrix && in);
        UHMatrix & operator=(UHMatrix && in);

        // FROM IMatrix2D
        size_type rows() const;
        size_type cols() const;
        double    operator()(size_type row, size_type col) const;
        double &  operator()(size_type row, size_type col);

        // Local methods
        void print() const;

    private:
        // to provide exception-safe copy-assignment
        void swap(UHMatrix const & in);

    private:
        // Number of columns
        size_type           ncols_;

        // data
        std::vector<double> data_;
    };

} // namespace LinAlg_NS

#pragma warning(default:4275)
#pragma warning(default:4251)
