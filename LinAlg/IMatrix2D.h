/*
 * Name  : IMatrix2D
 * Path  : 
 * Use   : Base class for matrix implementations (dense vs sparse)
 * Author: Sven Schmidt
 * Date  : 12/3/2011
 */
#pragma once

#include <boost/cstdint.hpp>

namespace LinAlg_NS {

    class Vector;


    class IMatrix2D {
    public:
        typedef boost::uint64_t size_type;

    public:
        // To be used as base class
        ~IMatrix2D() {}

        // Public matrix interface
        virtual size_type getRows() const                     = 0;
        virtual size_type getCols() const                     = 0;
        virtual double    operator()(size_type row, size_type col) const  = 0;
        virtual double &  operator()(size_type row, size_type col)        = 0;
        virtual void            solve(Vector const & b, Vector & x) const = 0;

        virtual void            print() const = 0;
    };

} // namespace LinAlg_NS
