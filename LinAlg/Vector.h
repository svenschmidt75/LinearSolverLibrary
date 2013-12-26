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


namespace boost {
    namespace serialization {
        class access;
    }

    namespace archive {
        class text_oarchive;
        class text_iarchive;
        class xml_oarchive;
        class xml_iarchive;
        class binary_oarchive;
        class binary_iarchive;
    }
}


#pragma warning(disable:4251)


namespace LinAlg_NS {

    namespace internal {

        template<typename MATRIX_EXPR, typename VECTOR_EXPR>
        class MatrixVectorExpr;
    }

    class LINALG_DECL_SYMBOLS Vector {

        // support for boost::serialize
        friend class boost::serialization::access;
        friend LINALG_DECL_SYMBOLS void serialize(boost::archive::text_oarchive & ar, Vector & m, const unsigned int version);
        friend LINALG_DECL_SYMBOLS void serialize(boost::archive::text_iarchive & ar, Vector & m, const unsigned int version);
        friend LINALG_DECL_SYMBOLS void serialize(boost::archive::xml_oarchive & ar, Vector & m, const unsigned int version);
        friend LINALG_DECL_SYMBOLS void serialize(boost::archive::xml_iarchive & ar, Vector & m, const unsigned int version);
        friend LINALG_DECL_SYMBOLS void serialize(boost::archive::binary_oarchive & ar, Vector & m, const unsigned int version);
        friend LINALG_DECL_SYMBOLS void serialize(boost::archive::binary_iarchive & ar, Vector & m, const unsigned int version);
        template<typename AR>
        friend void serialize_helper(AR & ar, Vector & m, const unsigned int /*version*/);

        // other
        friend class helper;

    public:
        typedef std::uint64_t                       size_type;
        typedef std::vector<double>::const_iterator const_iterator;
        typedef std::vector<double>::iterator       iterator;

    public:
        Vector();
        Vector(size_type dim);
        Vector(Vector const & in);
        Vector(Vector && in);

        template<typename VECTOR_EXPR, typename = typename std::enable_if<internal::entity_traits<VECTOR_EXPR>::is_vector_expression == true>::type>
        Vector(VECTOR_EXPR && in) {
            static_assert(typename internal::entity_traits<VECTOR_EXPR>::is_vector_expression == true, "in is not a vector-like type");
            dim_ = in.size();
            data_.resize(dim_);
            for (size_type i = 0; i < in.size(); ++i)
                (*this)(i) = in(i);
        }

        Vector & operator=(Vector const & in);
        Vector & operator=(Vector && in);

        double & operator()(size_type index);
        double   operator()(size_type index) const;

        size_type      size() const;
        void           clear();

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
