/*
 * Name  : TransposeVector
 * Path  : 
 * Use   : Implements a column-vector representation.
 *         This is implementation HAS-A Vector, only used
 *         to control operator overloading.
 * Author: Sven Schmidt
 * Date  : 07/28/2011
 */
#pragma once

#include "DeclSpec.h"

#include "Vector.h"

#include <boost/cstdint.hpp>


namespace LinAlg_NS {

    class TransposeVector {

        friend class helper;

    public:
        typedef boost::uint64_t size_type;

    public:
        TransposeVector(size_type dim);
        TransposeVector(TransposeVector const & in);
        TransposeVector & operator=(TransposeVector const & in);

        // enable move semantics
        TransposeVector(TransposeVector && in);
        TransposeVector & operator=(TransposeVector && in);


        double & operator()(size_type index);
        double   operator()(size_type index) const;

        size_type size() const;

    private:
        // to provide exception-safe copy-assignment
        void swap(TransposeVector const & in);

    private:
        Vector vector_;
    };

} // namespace LinAlg_NS
