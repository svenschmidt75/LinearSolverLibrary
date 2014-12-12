/* all common and stable header files */

#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <tuple>
#include <cstdint>

#pragma warning(disable:4512)
#include <boost/filesystem.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/assert.hpp>
#pragma warning(default:4512)


#include "LinAlg/IMatrix2D.h"
#include "LinAlg/SparseMatrix2D.h"
#include "LinAlg/Vector.h"

#include "FloridaSparseMatrixReaderLib/FloridaSparseMatrixReader.h"
#include "FloridaSparseMatrixReaderLib/FloridaSparseMatrixBuilder.h"
