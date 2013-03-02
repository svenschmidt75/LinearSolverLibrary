/*
 * Name  : ISymmetryStrategy
 * Path  : 
 * Use   : The Matrix Market format is different depending on whether
           the matrix is symmetric or not. This is the base class to
           implement the different strategies.
 * Author: Sven Schmidt
 * Date  : 03/02/2013
 */
#pragma once


#include "LinAlg/IMatrix2D.h"

#include <memory>
#include <string>


namespace EntityReader_NS {


    class ISymmetryStrategy {
    public:
        typedef std::unique_ptr<ISymmetryStrategy> UniquePtr;

    public:
        virtual ~ISymmetryStrategy() {}

        virtual void insert(LinAlg_NS::IMatrix2D::size_type row, LinAlg_NS::IMatrix2D::size_type col, double value) = 0;
        virtual bool check() const = 0;
    };

} // EntityReader_NS
