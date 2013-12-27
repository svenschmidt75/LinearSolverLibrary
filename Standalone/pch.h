/* all common and stable header files */

// enable BOOST_ASSERT
#undef NDEBUG

#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <cstdint>
#include <map>

#include <boost/filesystem.hpp>
#include <boost/assert.hpp>
#include <boost/format.hpp>

/* stable headers in other projects */
#include "FloridaSparseMatrixReaderLib/SparseMatrixReaderCreator.h"
#include "FloridaSparseMatrixReaderLib/VectorReaderCreator.h"

#include "LinearSolverLibrary/SparseLinearSolverLibrary.h"
#include "LinearSolverLibrary/SparseLinearSolverUtil.h"

#include "LinAlg/Vector.h"
#include "LinAlg/SparseMatrix2D.h"
#include "LinAlg/EntityOperators.h"

#include "LinearSolverLibrary/ConjugateGradientMethods.hpp"
