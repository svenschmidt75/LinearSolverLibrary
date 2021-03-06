/*
 * Name  : FloridaVectorBuilder
 * Path  : 
 * Use   : Builds a vector
 * Author: Sven Schmidt
 * Date  : 09/09/2012
 */
#pragma once

#include "DeclSpec.h"

#include "IVectorBuilder.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)


namespace EntityReader_NS {

    class FLORIDASPARSEMATRIXREADERLIB_DECL_SYMBOLS FloridaVectorBuilder : public IVectorBuilder<LinAlg_NS::Vector> {
    public:
        void     init(LinAlg_NS::Vector::size_type dim) const override final;
        void     insertVectorElement(LinAlg_NS::Vector::size_type row, double value) const override final;
        result_t result() const override final;

    private:
        mutable result_t                     data_;
        mutable LinAlg_NS::Vector::size_type dim_;
    };

} // namespace EntityReader_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
