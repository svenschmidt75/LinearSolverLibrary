/* all common and stable header files */

#include <string>
#include <cstdint>
#include <stdexcept>
#include <istream>
#include <map>
#include <set>
#include <vector>
#include <memory>
#include <cstdint>

#pragma warning(disable:4512)
#include <boost/algorithm/string.hpp>
#include <boost/progress.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/assert.hpp>
#include <boost/tokenizer.hpp>
#pragma warning(default:4512)

// warning C4702: unreachable code
#pragma warning(disable:4702)
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>
#pragma warning(default:4702)


#include "LinAlg/IMatrix2D.h"
#include "LinAlg/SparseMatrix2D.h"
#include "LinAlg/Vector.h"
#include "LinAlg/helper.h"

#include "common/reporting.h"
