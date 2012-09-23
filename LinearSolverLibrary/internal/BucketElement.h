/*
 * Name  : BucketElement
 * Path  : 
 * Use   : Represents a component x_i in a a sparse matrix
 * Author: Sven Schmidt
 * Date  : 09/22/2012
 */
#pragma once

#include "pch.h"


namespace LinearSolverLibrary_NS {

namespace internal_NS {

    class BucketElement {
    public:



    private:
        // dependencies of this x_i to other x_j
        typedef std::set<BucketElement> DependencyType;
        DependencyType dependencies_;

        // index of this element x_i in original sparse matrix
        LinAlg_NS::SparseMatrix2D::size_type prev_index_;

        // index of this element x_i in new sparse matrix
        LinAlg_NS::SparseMatrix2D::size_type index_;
    };

} // namespace internal_NS

} // namespace LinearSolverLibrary_NS
