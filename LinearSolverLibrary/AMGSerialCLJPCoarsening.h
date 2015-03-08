 /*
  * Name  : AMGSerialCLJPCoarsening
  * Path  :
  * Use   : Implements Cleary-Luby-Jones-Plassmann coarsening in serial
  * Author: 
  * Date  : 03/07/2015
  */
#pragma once

#include "DeclSpec.h"

#include "..\UnitTest/UnitTestForwardDeclarationHelper.h"


// forward declarations
namespace LinearSolverLibrary_NS {
    class IAMGStandardStrengthPolicy;
    class IVariableInfluenceAccessor;
    class VariableCategorizer;
}

// foward-declare test methods to avoid ambiguous symbols
GOOGLE_TEST(AMGSerialCLJPCoarseningTest, TestInitialWeights);



#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    class DECL_SYMBOLS AMGSerialCLJPCoarsening {


        // make tests friends
        FRIEND_GOOGLE_TEST(AMGSerialCLJPCoarseningTest, TestInitialWeights);


    public:
        using size_type = LinAlg_NS::IMatrix2D::size_type;

    public:
        AMGSerialCLJPCoarsening(LinAlg_NS::SparseMatrix2D const & m,
            IAMGStandardStrengthPolicy const & strength_policy,
            IVariableInfluenceAccessor const & variable_influence_accessor,
            VariableCategorizer & categorizer);

        AMGSerialCLJPCoarsening(AMGSerialCLJPCoarsening const &) = delete;
        AMGSerialCLJPCoarsening & operator=(AMGSerialCLJPCoarsening const &) = delete;

        void coarsen();

    private:
        void                   initialWeightInitialization();
        std::vector<size_type> selectIndependentSet();
        void                   updateWeights();

    private:
        using Weight_t = std::map<size_type, double>;

        LinAlg_NS::SparseMatrix2D const &  m_;
        IAMGStandardStrengthPolicy const & strength_policy_;
        IVariableInfluenceAccessor const & variable_influence_accessor_;
        VariableCategorizer &              categorizer_;
        Weight_t                           weights_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
