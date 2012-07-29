/*
 * Name  : Vector
 * Path  : 
 * Use   : Implements a row-vector representation.
 *         We are introducing a new type here, although
 *         we could have worked with a std::vector. However,
 *         when using operator overloading, for example for
 *         a matrix-vector multiplication, it is advised
 *         to create a separate vector type.
 * Author: Sven Schmidt
 * Date  : 07/28/2011
 */
#pragma once

#include "DeclSpec.h"

#include <vector>

#include <boost/cstdint.hpp>


#pragma warning(disable:4251)


namespace LinAlg_NS {

    class DECL_SYMBOLS Vector {

        friend class helper;

    public:
        typedef boost::uint64_t size_type;

    public:
        Vector(size_type dim);
        Vector(Vector const & in);
        Vector & operator=(Vector const & in);

        // enable move semantics
        Vector(Vector && in);
        Vector & operator=(Vector && in);

        template<typename EXPR>
        Vector & operator=(EXPR const & in) {
            data_.resize(in.size());

            for (Vector::size_type i = 0; i < in.size(); ++i)
                (*this)(i) = in(i);

            return *this;
        }

        double & operator()(size_type index);
        double   operator()(size_type index) const;

        size_type size() const;

    private:
        // to provide exception-safe copy-assignment
        void swap(Vector const & in);

    private:
        size_type           dim_;
        std::vector<double> data_;
    };

} // namespace LinAlg_NS

#pragma warning(default:4251)
