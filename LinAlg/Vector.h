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

#include "internal/expression_traits.h"

#include <vector>

#include <boost/cstdint.hpp>


#pragma warning(disable:4251)


namespace LinAlg_NS {

    class DECL_SYMBOLS Vector {

        friend class helper;

    public:
        typedef boost::uint64_t                     size_type;
        typedef std::vector<double>::const_iterator const_iterator;
        typedef std::vector<double>::iterator       iterator;

    public:
        Vector(size_type dim);
        Vector(Vector const & in);
        Vector & operator=(Vector const & in);

        // enable move semantics
        Vector(Vector && in);
        Vector & operator=(Vector && in);

        template<typename VECTOR_EXPR>
        Vector & operator=(VECTOR_EXPR const & in) {
            static_assert(typename expression_traits<VECTOR_EXPR>::is_vector_expression::value == std::true_type::value, "rhs is not a vector-like type");

            data_.resize(in.size());

            for (Vector::size_type i = 0; i < in.size(); ++i)
                (*this)(i) = in(i);

            return *this;
        }

        double & operator()(size_type index);
        double   operator()(size_type index) const;

        size_type size() const;

        const_iterator cbegin() const;
        const_iterator cend() const;
        iterator       begin();
        iterator       end();

    private:
        // to provide exception-safe copy-assignment
        void swap(Vector const & in);

    private:
        size_type           dim_;
        std::vector<double> data_;
    };

} // namespace LinAlg_NS

#pragma warning(default:4251)
