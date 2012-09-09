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

#include "LinAlg/Vector.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)


namespace EntityReader_NS {

    class FLORIDASPARSEMATRIXREADERLIB_DECL_SYMBOLS FloridaVectorBuilder : public IVectorBuilder<LinAlg_NS::Vector> {
    public:
        void     init(LinAlg_NS::Vector::size_type dim) const final;
        void     insertVectorElement(LinAlg_NS::Vector::size_type row, double value) const final;
        result_t result() const final;

    private:
        mutable result_t                     data_;
        mutable LinAlg_NS::Vector::size_type dim_;
    };

} // namespace EntityReader_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
