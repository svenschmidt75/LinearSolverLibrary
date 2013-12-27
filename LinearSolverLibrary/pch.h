/* all common and stable header files */

#include <iostream>
#include <tuple>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <forward_list>
#include <set>
#include <numeric>
#include <cstdint>
#include <map>
#include <queue>

#include <boost/math/special_functions/round.hpp>
#include <boost/assert.hpp>
#include <boost/noncopyable.hpp>

/* stable headers in other projects */

#include "common/reporting.h"
#include "common/InputIterator.hpp"
#include "common/IInputIteratorLogic.h"

#include "LinAlg/EntityOperators.h"
#include "LinAlg/Matrix2D.h"
#include "LinAlg/SparseMatrix2D.h"
#include "LinAlg/UHMatrix.h"
#include "LinAlg/MatrixIterators.h"
#include "LinAlg/ConstRowColumnIterator.h"
#include "LinAlg/ConstColumnIterator.h"
#include "LinAlg/VectorMath.h"
