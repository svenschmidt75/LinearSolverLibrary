/*
 * Name  : ScalarExpression
 * Path  : 
 * Use   : Encapsulates a scalar value.
 * 
 *         Note: Because we provide an implementation in a cpp file,
 *         the compiler cannot inline the methods!!!
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
            ScalarExpression(double value);
            ScalarExpression(ScalarExpression const & in);

            double operator()(Vector::size_type index) const;

        private:
            double value_;
        };


    } // namespace internal

} // namespace LinAlg_NS
