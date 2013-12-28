/* all common and stable header files */

// enable BOOST_ASSERT
#undef NDEBUG

#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <cstdint>
#include <map>
#include <set>

#include <boost/filesystem.hpp>
#include <boost/assert.hpp>
#include <boost/format.hpp>

/* stable headers in other projects */

#include "common/reporting.h"

#include "LinAlg/Vector.h"
#include "LinAlg/SparseMatrix2D.h"
#include "LinAlg/EntityOperators.h"
#include "LinAlg/entity_operation_traits.hpp"
#include "LinAlg/VectorMath.h"

#include "FloridaSparseMatrixReaderLib/SparseMatrixReaderCreator.h"
#include "FloridaSparseMatrixReaderLib/VectorReaderCreator.h"

#include "LinearSolverLibrary/SparseLinearSolverLibrary.h"
#include "LinearSolverLibrary/SparseLinearSolverUtil.h"
#include "LinearSolverLibrary/ConjugateGradientMethods.hpp"
