#include "pch.h"

#include "AMGStrengthPolicyImpl.h"
#include "VariableSet.h"
#include <common/Utility.hpp>


using namespace LinAlg_NS;
using namespace LinearSolverLibrary_NS;


std::unique_ptr<IVariableSet>
AMGStrengthPolicyImpl::getNeighborhood(IMatrix2D::size_type variable) const {
    auto variable_set = std::make_unique<VariableSet>();

    auto influencers = getStrongInfluencers(variable);
    for (auto node : *influencers)
        variable_set->add(node);

    auto influenced = getStronglyInfluenced(variable);
    for (auto node : *influenced)
        variable_set->add(node);

    return common_NS::convert<IVariableSet>(variable_set);
}
