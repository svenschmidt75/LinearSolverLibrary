/*
 * Name  : PrimitiveOperators
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 07/29/2011
 */
#pragma once

namespace LinAlg_NS {

    namespace internal {

        template<typename T>
        struct ADD {
            auto operator()(T const & lhs, T const & rhs) const -> decltype(lhs + rhs) {
                return lhs + rhs;
            }
        };

        template<typename T>
        struct SUB {
            auto operator()(T const & lhs, T const & rhs) const -> decltype(lhs - rhs) {
                return lhs - rhs;
            }
        };

        template<typename T>
        struct MUL {
            auto operator()(T const & lhs, T const & rhs) const -> decltype(lhs * rhs) {
                return lhs * rhs;
            }
        };

    } // namespace internal

} // namespace LinAlg_NS
