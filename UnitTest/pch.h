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

#include <ppl.h>

/* stable headers in other projects */

#include "common/reporting.h"
#include "common/InputIterator.hpp"

#include "LinAlg/EntityOperators.h"
#include "LinAlg/Matrix2D.h"
#include "LinAlg/SparseMatrix2D.h"
#include "LinAlg/VectorMath.h"

#include "FloridaSparseMatrixReaderLib/SparseMatrixReaderCreator.h"
#include "FloridaSparseMatrixReaderLib/VectorReaderCreator.h"

#include "LinearSolverLibrary/SparseLinearSolverLibrary.h"
#include "LinearSolverLibrary/SparseLinearSolverUtil.h"
