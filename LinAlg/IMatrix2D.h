/*
 * Name  : IMatrix2D
 * Path  : 
 * Use   : Base class for matrix implementations (dense vs sparse)
 * Author: Sven Schmidt
 * Date  : 12/3/2011
 */
#pragma once


namespace LinAlg_NS {

    class Vector;


    class IMatrix2D {
    public:
        using size_type = std::uint64_t;

    public:
        // To be used as base class
        ~IMatrix2D() {}

        // Public matrix interface
        virtual size_type rows() const = 0;
        virtual size_type cols() const = 0;
        virtual double    operator()(size_type row, size_type col) const = 0;
        virtual double &  operator()(size_type row, size_type col)       = 0;

        virtual void      print() const = 0;
    };

} // namespace LinAlg_NS
