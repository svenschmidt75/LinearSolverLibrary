#include "pch.h"

#include "AMGSerialCLJPCoarsening.h"

namespace LinearSolverLibrary_NS{
class IAMGStandardStrengthPolicy;
}

using namespace LinearSolverLibrary_NS;
using namespace LinAlg_NS;


AMGSerialCLJPCoarsening::AMGSerialCLJPCoarsening(SparseMatrix2D const & m, IAMGStandardStrengthPolicy const & strength_policy, IVariableInfluenceAccessor const & variable_influence_accessor, VariableCategorizer & categorizer)
    :
    m_{m},
    strength_policy_{strength_policy},
    variable_influence_accessor_{variable_influence_accessor},
    categorizer_{categorizer} {}

void
AMGSerialCLJPCoarsening::coarsen() {
    initialWeightInitialization();
    split();
}

void
AMGSerialCLJPCoarsening::initialWeightInitialization() {
    std::random_device rd;
    std::mt19937 random_number_generator(rd());
    std::uniform_real_distribution<> distribution(0, 1);

    for (size_type i = 0; i < m_.cols(); ++i) {
        // Get the number of variables that variable i strongly influences.
        // The more there are, the more likely i is to become a coarse variable.
        auto strongly_influenced = strength_policy_.getStronglyInfluenced(i);

        // generate a random number, so no two variables with the same number of
        // other variables that they influence, end up with the same weight
        auto rnd = distribution(random_number_generator);

        weights_[i] = strongly_influenced->size() + rnd;
    }
}

std::vector<AMGSerialCLJPCoarsening::size_type>
AMGSerialCLJPCoarsening::selectIndependentSet() {
    std::vector<size_type> independent_set;
    return independent_set;
}

void
AMGSerialCLJPCoarsening::updateWeights() {
    
}

void
AMGSerialCLJPCoarsening::split() {
    std::forward_list<size_type> unassigned_variables(m_.cols());
    std::iota(std::begin(unassigned_variables), std::end(unassigned_variables), 0);

    while (unassigned_variables.empty() == false) {
        auto current_variable = unassigned_variables.front();
        unassigned_variables.pop_front();

        auto independent_set = getIndependentSet(current_variable);



    }




}

std::vector<AMGSerialCLJPCoarsening::size_type>
AMGSerialCLJPCoarsening::getIndependentSet(size_type current_variable) const {
    std::vector<size_type> independent_set;
    for (size_type i = 0; i < m_.cols(); ++i) {
        auto weight = weights_.at(current_variable);

        auto influencers = strength_policy_.getStrongInfluencers(i);
        auto influenced = strength_policy_.getStronglyInfluenced(i);

        for (auto const & item : *influencers) {
            auto other_variable = item;
        }



    }


    return independent_set;
}