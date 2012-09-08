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


namespace common_NS {

    class COMMON_DECL_SYMBOLS reporting {
    public:
        static bool error(std::string const & msg);
    };

} // namespace common_NS
