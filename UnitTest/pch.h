/* all common and stable header files */

#include <string>
#include <numeric>
#include <iostream>
#include <tuple>
#include <stdexcept>
#include <thread>
#include <random>
#include <forward_list>
#include <cstdint>
#include <map>
#include <queue>
#include <algorithm>

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestResult.h>
#include <cppunit/BriefTestProgressListener.h>

#include <boost/filesystem.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/chrono.hpp>
#include <boost/format.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <ppl.h>

#include <gmock/gmock.h>

/* stable headers in other projects */

#include "common/reporting.h"
#include "common/InputIterator.hpp"
#include "common/parallel.hpp"

#include "LinAlg/EntityOperators.h"
#include "LinAlg/entity_operation_traits.hpp"
#include "LinAlg/helper.h"
#include "LinAlg/Matrix2D.h"
#include "LinAlg/SparseMatrix2D.h"
#include "LinAlg/VectorMath.h"
#include "LinAlg/DirichletBoundaryConditionPolicy.hpp"
#include "LinAlg/PeriodicBoundaryConditionPolicy.hpp"
#include "LinAlg/MatrixIterators.h"
#include "LinAlg/ConstRowColumnIterator.h"
#include "LinAlg/ConstColumnIterator.h"
#include "LinAlg/ConstRowColumnIteratorGeneric.hpp"
#include "LinAlg/ConstColumnIteratorGeneric.hpp"
#include "LinAlg/UHMatrix.h"
#include "LinAlg/UTMatrix.h"
#include "LinAlg/MatrixStencil.hpp"

#include "FloridaSparseMatrixReaderLib/SparseMatrixReaderCreator.h"
#include "FloridaSparseMatrixReaderLib/VectorReaderCreator.h"
#include "FloridaSparseMatrixReaderLib/internal/ISymmetryStrategy.h"
#include "FloridaSparseMatrixReaderLib/internal/SymmetryStrategyFactory.h"

#include "LinearSolverLibrary/SparseLinearSolverLibrary.h"
#include "LinearSolverLibrary/SparseLinearSolverUtil.h"
#include "LinearSolverLibrary/VariableCategorizer.h"
#include "LinearSolverLibrary/VariableCardinalityPolicy.h"
#include "LinearSolverLibrary/VariableInfluenceAccessor.h"
#include "LinearSolverLibrary/GaussJordan.h"
#include "LinearSolverLibrary/ConjugateGradientMethods.hpp"
#include "LinearSolverLibrary/VariableSet.h"
#include "LinearSolverLibrary/internal/BucketList.h"
#include "LinearSolverLibrary/internal/BucketElement.h"
#include "LinearSolverLibrary/LUDecomposition.h"
#include "LinearSolverLibrary/AMGCThenFRelaxationPolicy.hpp"
#include "LinearSolverLibrary/AMGHierarchyBuilder.hpp"
#include "LinearSolverLibrary/AMGMonitor.h"
#include "LinearSolverLibrary/AMGStandardStrengthPolicy.h"
#include "LinearSolverLibrary/AMGDirectInterpolationPolicy.h"
#include "LinearSolverLibrary/AMGStandardInterpolationPolicy.h"
#include "LinearSolverLibrary/AMGStandardCoarsening.h"
#include "LinearSolverLibrary/AMGVCycle.h"
#include "LinearSolverLibrary/AMGWCycle.hpp"
//#include "LinearSolverLibrary/AMGSolver.hpp"
