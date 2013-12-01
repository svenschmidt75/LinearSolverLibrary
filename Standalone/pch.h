/* all common and stable header files */

// enable BOOST_ASSERT
#undef NDEBUG

/* stable headers in other projects */
#include "FloridaSparseMatrixReaderLib/SparseMatrixReaderCreator.h"
#include "FloridaSparseMatrixReaderLib/VectorReaderCreator.h"

#include "LinearSolverLibrary/SparseLinearSolverLibrary.h"
#include "LinearSolverLibrary/SparseLinearSolverUtil.h"

#include "LinAlg/Vector.h"
#include "LinAlg/SparseMatrix2D.h"
#include "LinAlg/EntityOperators.h"

#include "LinearSolverLibrary/ConjugateGradientMethods.hpp"


#include <boost/filesystem.hpp>
#include <boost/assert.hpp>

#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
