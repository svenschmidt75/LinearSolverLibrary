/*
 * Name  : reporting
 * Path  : 
 * Use   : Common reporting functionality other projects depend on.
 * Author: Sven Schmidt
 * Date  : 09/08/2012
 */
#pragma once

#include "DeclSpec.h"


namespace common_NS {

    class COMMON_DECL_SYMBOLS reporting {
    public:
        static bool error(std::string const & msg);

        template<typename T, typename OP>
        static void checkBound(T value, T maxValue, std::string const & error_message) {
            bool condition = OP()(value, maxValue);
            if (!condition) {
                boost::format format = boost::format(error_message);
                format.str();
                throw std::runtime_error(format.str());
            }
        }

        template<typename T>
        static void checkLowerBound(T value, T maxValue, std::string const & error_message = "bound check failure\n") {
            checkBound<T, std::greater_equal<T>>(value, maxValue, error_message);
        }

        template<typename T>
        static void checkUppderBound(T value, T maxValue, std::string const & error_message = "bound check failure\n") {
            checkBound<T, std::less_equal<T>>(value, maxValue, error_message);
        }

        static void checkConditional(bool condition, std::string const & error_message = "bound check failure\n") {
            if (!condition) {
                boost::format format = boost::format(error_message);
                format.str();
                throw std::runtime_error(format.str());
            }
        }

    };

} // namespace common_NS

