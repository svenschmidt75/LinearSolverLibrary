/*
* Name  : AMGStandardCoarsening
* Path  :
* Use   : Compute C/F splitting based on direct couplings as
          described in Trottenberg, page 473, A.7.1.1
* Author: Sven Schmidt
* Date  : 10/19/2013
*/
#pragma once

#include "DeclSpec.h"

#include "AMGStandardCoarseningStrengthPolicy.h"
#include "VariableCategorizer.h"

#include "LinAlg/SparseMatrix2D.h"

#include <boost/noncopyable.hpp>


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS AMGStandardCoarsening : boost::noncopyable {
    public:
        AMGStandardCoarsening(LinAlg_NS::SparseMatrix2D const & m, VariableCategorizer & categorizer);

    private:
        LinAlg_NS::SparseMatrix2D const &   m_;
        VariableCategorizer &               categorizer_;
        AMGStandardCoarseningStrengthPolicy strength_policy_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
