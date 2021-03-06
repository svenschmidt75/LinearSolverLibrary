﻿ /*
  * Name  : AMGSerialCLJPCoarsening
  * Path  :
  * Use   : Implements Cleary-Luby-Jones-Plassmann coarsening in serial
  * Author: 
  * Date  : 03/07/2015
  */
#pragma once

#include "DeclSpec.h"

#include "..\UnitTest/UnitTestForwardDeclarationHelper.h"
#include "AMGStrengthGraph.h"


class AMGSerialCLJPCoarseningAlberDissPage25Test;

// forward declarations
namespace LinearSolverLibrary_NS {
    class AMGStrengthPolicyImpl;
    class IVariableInfluenceAccessor;
    class VariableCategorizer;
}

// foward-declare test methods to avoid ambiguous symbols
GOOGLE_TEST(AMGSerialCLJPCoarseningTest, TestInitialWeights);
GOOGLE_TEST(AMGSerialCLJPCoarseningTest, TestIndependentSet);
GOOGLE_TEST(AMGSerialCLJPCoarseningTest, TestWeightUpdate);
GOOGLE_TEST(AMGSerialCLJPCoarseningTest, TestWeightUpdateHeuristic1);
GOOGLE_TEST(AMGSerialCLJPCoarseningTest, TestWeightUpdateHeuristic2);
class AMGSerialCLJPCoarseningAlberDissPage25Test;
GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestInitialIndependentSet);
GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestWeightUpdateForNode9);
GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestWeightUpdateForNode10);
GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestWeightUpdateForNode20);
GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestWeightUpdateForNode22);
GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestWeightsAroundNode9);
GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestWeightsAroundNode10);
GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestWeightsAroundNode20);
GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestWeightsAroundNode22);
GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestWeightsAroundNode9And10);
GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestWeightsAroundNode20And22);
GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestEdgesOfNode18);
GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestFineNodestUpdateForNode20);
GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestFineNodestUpdateForNode22);
GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestAfterStep1);
GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestAfterStep2);
GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestAfterStep3);
GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestFullCoarsening);

class AMGSerialCLJPCoarseningButlerDissPage56Test;
GOOGLE_TEST(AMGSerialCLJPCoarseningButlerDissPage56Test, TestInitialIndependentSet);
GOOGLE_TEST(AMGSerialCLJPCoarseningButlerDissPage56Test, TestEdgesOfNode5);
GOOGLE_TEST(AMGSerialCLJPCoarseningButlerDissPage56Test, TestAfterStep1);
GOOGLE_TEST(AMGSerialCLJPCoarseningButlerDissPage56Test, TestAfterStep2);
GOOGLE_TEST(AMGSerialCLJPCoarseningButlerDissPage56Test, TestAfterStep3);
GOOGLE_TEST(AMGSerialCLJPCoarseningButlerDissPage56Test, TestAfterStep4);
GOOGLE_TEST(AMGSerialCLJPCoarseningButlerDissPage56Test, TestFullCoarsening);



#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    class LINEAR_SOLVER_LIBRARY_DECL_SYMBOLS AMGSerialCLJPCoarsening {


        // make tests friends
        FRIEND_GOOGLE_TEST(AMGSerialCLJPCoarseningTest, TestInitialWeights);
        FRIEND_GOOGLE_TEST(AMGSerialCLJPCoarseningTest, TestIndependentSet);
        FRIEND_GOOGLE_TEST(AMGSerialCLJPCoarseningTest, TestWeightUpdate);
        FRIEND_GOOGLE_TEST(AMGSerialCLJPCoarseningTest, TestWeightUpdateHeuristic1);
        FRIEND_GOOGLE_TEST(AMGSerialCLJPCoarseningTest, TestWeightUpdateHeuristic2);
        friend class AMGSerialCLJPCoarseningAlberDissPage25Test;
        FRIEND_GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestInitialIndependentSet);
        FRIEND_GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestWeightUpdateForNode9);
        FRIEND_GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestWeightUpdateForNode10);
        FRIEND_GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestWeightUpdateForNode20);
        FRIEND_GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestWeightUpdateForNode22);
        FRIEND_GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestWeightsAroundNode9);
        FRIEND_GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestWeightsAroundNode10);
        FRIEND_GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestWeightsAroundNode20);
        FRIEND_GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestWeightsAroundNode22);
        FRIEND_GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestWeightsAroundNode9And10);
        FRIEND_GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestWeightsAroundNode20And22);
        FRIEND_GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestEdgesOfNode18);
        FRIEND_GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestFineNodestUpdateForNode20);
        FRIEND_GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestFineNodestUpdateForNode22);
        FRIEND_GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestAfterStep1);
        FRIEND_GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestAfterStep2);
        FRIEND_GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestAfterStep3);
        FRIEND_GOOGLE_TEST(AMGSerialCLJPCoarseningAlberDissPage25Test, TestFullCoarsening);

        friend class AMGSerialCLJPCoarseningButlerDissPage56Test;
        FRIEND_GOOGLE_TEST(AMGSerialCLJPCoarseningButlerDissPage56Test, TestInitialIndependentSet);
        FRIEND_GOOGLE_TEST(AMGSerialCLJPCoarseningButlerDissPage56Test, TestEdgesOfNode5);
        FRIEND_GOOGLE_TEST(AMGSerialCLJPCoarseningButlerDissPage56Test, TestAfterStep1);
        FRIEND_GOOGLE_TEST(AMGSerialCLJPCoarseningButlerDissPage56Test, TestAfterStep2);
        FRIEND_GOOGLE_TEST(AMGSerialCLJPCoarseningButlerDissPage56Test, TestAfterStep3);
        FRIEND_GOOGLE_TEST(AMGSerialCLJPCoarseningButlerDissPage56Test, TestAfterStep4);
        FRIEND_GOOGLE_TEST(AMGSerialCLJPCoarseningButlerDissPage56Test, TestFullCoarsening);




    public:
        using size_type = LinAlg_NS::IMatrix2D::size_type;

    public:
        AMGSerialCLJPCoarsening(LinAlg_NS::SparseMatrix2D const & m,
            AMGStrengthPolicyImpl const & strength_policy,
            IVariableInfluenceAccessor const & variable_influence_accessor,
            VariableCategorizer & categorizer);

        AMGSerialCLJPCoarsening(AMGSerialCLJPCoarsening const &) = delete;
        AMGSerialCLJPCoarsening & operator=(AMGSerialCLJPCoarsening const &) = delete;

        void coarsen();

        void exportToGraphviz(std::string const & filename) const;

    private:
        void                   initialWeightInitialization();
        void                   updateWeights(size_type variable);
        std::vector<size_type> selectIndependentSet() const;
        void                   setFineNodes(size_type j);
        void                   printWeights() const;

    private:
        using Weight_t = std::map<size_type, double>;

        LinAlg_NS::SparseMatrix2D const &  m_;
        AMGStrengthPolicyImpl const &      strength_policy_;
        AMGStrengthGraph                   strength_graph_;
        IVariableInfluenceAccessor const & variable_influence_accessor_;
        VariableCategorizer &              categorizer_;
        Weight_t                           weights_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
