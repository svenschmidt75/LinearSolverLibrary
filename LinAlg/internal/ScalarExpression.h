/*
 * Name  : ScalarExpression
 * Path  : 
 * Use   : Encapsulates a double
 * Author: Sven Schmidt
 * Date  : 07/29/2011
 */
#pragma once

#include "DeclSpec.h"

#include "Vector.h"


namespace LinAlg_NS {

    namespace internal {

        class DECL_SYMBOLS ScalarExpression {
        public:
            ScalarExpression(double value, Vector::size_type dim);
            ScalarExpression(ScalarExpression const & in);

            Vector::size_type size() const;

            double operator()(Vector::size_type index) const;

        private:
            double            value_;
            Vector::size_type dim_;
        };


    } // namespace internal

} // namespace LinAlg_NS
