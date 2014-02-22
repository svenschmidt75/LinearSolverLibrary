#include "pch.h"


#include "LinearSolverLibrary/VariableCategorizer.h"
#include "LinearSolverLibrary/AMGCoarseVariableIndexer.h"



using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;
using namespace testing;


TEST(CoarseVariableIndexerTest, TestSimpleMapping) {
    VariableCategorizer categorizer{1};
    categorizer.SetType(0, VariableCategorizer::Type::COARSE);
    AMGCoarseVariableIndexer mapper{categorizer};
    ASSERT_EQ(mapper.Index(0), 0);
}

TEST(CoarseVariableIndexerTest, TestGetIndexForInvalidThrows) {
    unsigned numberOfVariables{5};
    VariableCategorizer categorizer{numberOfVariables};
    categorizer.SetType(0, VariableCategorizer::Type::COARSE);
    AMGCoarseVariableIndexer mapper{categorizer};
    ASSERT_THROW(mapper.Index(2), std::out_of_range);
}

TEST(CoarseVariableIndexerTest, TestGetIndex) {
    unsigned numberOfVariables{5};
    VariableCategorizer categorizer{numberOfVariables};
    categorizer.SetType(0, VariableCategorizer::Type::COARSE);
    categorizer.SetType(2, VariableCategorizer::Type::COARSE);
    categorizer.SetType(4, VariableCategorizer::Type::COARSE);
    AMGCoarseVariableIndexer mapper{categorizer};
    ASSERT_EQ(mapper.Index(0), 0);
    ASSERT_EQ(mapper.Index(2), 1);
    ASSERT_EQ(mapper.Index(4), 2);
}

TEST(CoarseVariableIndexerTest, TestGetIndexForNonExistentThrows) {
    unsigned numberOfVariables{5};
    VariableCategorizer categorizer{numberOfVariables};
    categorizer.SetType(0, VariableCategorizer::Type::COARSE);
    categorizer.SetType(2, VariableCategorizer::Type::COARSE);
    categorizer.SetType(4, VariableCategorizer::Type::COARSE);
    AMGCoarseVariableIndexer mapper{categorizer};
    ASSERT_THROW(mapper.Index(1), std::out_of_range);
    ASSERT_THROW(mapper.Index(3), std::out_of_range);
}

TEST(CoarseVariableIndexerTest, TestNumberOfVariables) {
    unsigned numberOfVariables{5};
    VariableCategorizer categorizer{numberOfVariables};
    categorizer.SetType(0, VariableCategorizer::Type::COARSE);
    categorizer.SetType(2, VariableCategorizer::Type::COARSE);
    categorizer.SetType(4, VariableCategorizer::Type::COARSE);
    AMGCoarseVariableIndexer mapper{categorizer};
    ASSERT_EQ(mapper.NumberOfVariables(), 3);
}
