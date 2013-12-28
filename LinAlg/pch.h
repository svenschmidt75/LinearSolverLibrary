/* all common and stable header files */

#include <boost/assert.hpp>
#include <boost/cstdint.hpp>
#include <boost/mpl/bool_fwd.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/format.hpp>

#include <cassert>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <utility>
#include <cmath>
#include <algorithm>
#include <map>
#include <vector>
#include <cstdint>
#include <thread>

#include <ppl.h>

/* stable headers in other projects */

#include "common/reporting.h"
#include "common/parallel.hpp"
