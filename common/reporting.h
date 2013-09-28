/*
 * Name  : reporting
 * Path  : 
 * Use   : Common reporting functionality other projects depend on.
 * Author: Sven Schmidt
 * Date  : 09/08/2012
 */
#pragma once

#include "DeclSpec.h"

#include <string>
#include <functional>


namespace common_NS {

    class COMMON_DECL_SYMBOLS reporting {
    public:
        static bool error(std::string const & msg);

        template<typename T, typename OP>
        static void checkBound(T value, T maxValue) {
            bool condition = OP()(value, maxValue);
            BOOST_ASSERT_MSG(condition, "bound check failure");
            if (!condition) {
                boost::format format = boost::format("bound check failure\n");
                (format.str());
                throw std::out_of_range(format.str());
            }
        }

        template<typename T>
        static void checkLowerBound(T value, T maxValue) {
            checkBound<T, std::greater_equal<T>>(value, maxValue);
        }

        template<typename T>
        static void checkUppderBound(T value, T maxValue) {
            checkBound<T, std::less_equal<T>>(value, maxValue);
        }

    };

} // namespace common_NS
