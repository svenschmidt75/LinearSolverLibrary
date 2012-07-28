/*
 * Name  : Vector
 * Path  : 
 * Use   : Implements a vector representation
 * Author: Sven Schmidt
 * Date  : 07/28/2011
 */
#pragma once

#include "DeclSpec.h"

#include <vector>

#include <boost/cstdint.hpp>


namespace LinAlg_NS {

    class Vector {
    public:
        typedef boost::uint64_t size_type;

    public:
        Vector(size_type dim);
        Vector(Vector const & in);
        Vector & operator=(Vector const & in);

        // enable move semantics
        Vector(Vector const && in);
        Vector & operator=(Vector const && in);


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