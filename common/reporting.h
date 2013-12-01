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

#ifndef DEBUG
  #pragma warning(disable:4100)
#endif

namespace common_NS {

    class COMMON_DECL_SYMBOLS reporting {
    public:
        static bool error(std::string const & msg);

        template<typename T, typename OP>
        static void checkBound(T value, T maxValue, std::string const & error_message) {
#ifdef DEBUG
            bool condition = OP()(value, maxValue);
            if (!condition) {
                boost::format format = boost::format(error_message);
                format.str();
                throw std::runtime_error(format.str());
            }
#endif
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
#ifdef DEBUG
            if (!condition) {
                boost::format format = boost::format(error_message);
                format.str();
                throw std::runtime_error(format.str());
            }
#endif
        }

    };

} // namespace common_NS

#ifndef DEBUG
#pragma warning(default:4100)
#endif

