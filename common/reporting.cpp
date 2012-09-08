#include "reporting.h"

#include <iostream>

using namespace common_NS;


bool
reporting::error(std::string const & msg) {
    std::cerr << msg << std::endl;
    return false;
}
