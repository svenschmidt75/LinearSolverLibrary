/*
 * Name  : IAMGStrengthPolicy
 * Path  :
 * Use   : Assembles the strength matrix S.
           Determines strong connections of a variable on others, i.e.
           S_{i} and S_{i}^{T}, see Trottenberg, page 473, 474.
           The strength variable i is connected to variables j is measured
           based on the smallest negative value a_{ij} * eps.
 * Author: Sven Schmidt
 * Date  : 06/14/2014
 */
#pragma once


// forward declarations
namespace LinearSolverLibrary_NS {
    class IVariableSet;
}


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    class IAMGStrengthPolicy {
    public:
        virtual ~IAMGStrengthPolicy() {}

        virtual std::unique_ptr<IVariableSet> getStrongInfluencers(LinAlg_NS::IMatrix2D::size_type variable) const = 0;
        virtual std::unique_ptr<IVariableSet> getStronglyInfluenced(LinAlg_NS::IMatrix2D::size_type variable) const = 0;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
