/*
 * Name  : IMatrixBuilder
 * Path  : 
 * Use   : Base class for a matrix builder object
 * Author: Sven Schmidt
 * Date  : 08/12/2012
 */
#pragma once

#include "LinAlg/IMatrix2D.h"

#include <memory>
#include <string>


namespace EntityReader_NS {

    template<typename T>
    class IMatrixBuilder {
    public:
        using result_t = std::shared_ptr<T>;

    public:
        virtual ~IMatrixBuilder() {}

        virtual void     init(LinAlg_NS::IMatrix2D::size_type rows, LinAlg_NS::IMatrix2D::size_type cols, std::string const & symmetry_strategy) const = 0;
        virtual void     insertMatrixElement(LinAlg_NS::IMatrix2D::size_type row, LinAlg_NS::IMatrix2D::size_type col, double value) const = 0;
        virtual void     finalize() const = 0;
        virtual result_t result() const = 0;
    };

} // namespace EntityReader_NS
