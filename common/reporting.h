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
#include <stdexcept>

#include <boost/assert.hpp>
#include <boost/format.hpp>


namespace common_NS {

    class COMMON_DECL_SYMBOLS reporting {
    public:
        static bool error(std::string const & msg);

        template<typename T, typename OP>
        static void checkBound(T value, T maxValue) {
            bool condition = OP()(value, maxValue);
            if (!condition) {
                boost::format format = boost::format("bound check failure\n");
                format.str();
                throw std::runtime_error(format.str());
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

        static void checkConditional(bool condition) {
            if (!condition) {
                boost::format format = boost::format("condition failure\n");
                format.str();
                throw std::runtime_error(format.str());
            }
        }

    };

} // namespace common_NS
