/*
 * Name  : BucketElement
 * Path  : 
 * Use   : Represents a component x_i in a a sparse matrix
 * Author: Sven Schmidt
 * Date  : 09/22/2012
 */
#pragma once

#include "..\DeclSpec.h"


#pragma warning(disable:4251)


namespace LinearSolverLibrary_NS {

namespace internal_NS {

    class LINEAR_SOLVER_LIBRARY_DECL_SYMBOLS BucketElement {

        friend class BucketList;

    public:
        typedef std::shared_ptr<BucketElement> Ptr;
        typedef std::uint64_t size_type;

    private:
        typedef std::set<Ptr> DependencyType;

        struct comparitor {
            comparitor() {}

            bool operator()(Ptr const & lhs, Ptr const & rhs) const {
                return lhs->prevIndex() < rhs->prevIndex();
            }
        };

    public:
        typedef DependencyType::const_iterator const_iterator;
        typedef DependencyType::iterator iterator;
        typedef std::uint64_t color_t;

    public:
        BucketElement(size_type element_index);

        const_iterator  cbegin() const;
        const_iterator  cend() const;

        iterator        begin() const;
        iterator        end() const;

        void            dependsOn(Ptr element_index);
        const_iterator  findDependency(BucketElement const & element) const;
        bool            findDependency(size_type index) const;

        void            topologicalIndex(size_type index);
        size_type       topologicalIndex() const;

        size_type       prevIndex() const;

        color_t         color() const;
        void            color(color_t color);

    private:
        // dependencies of this x_i to other x_j
        DependencyType dependencies_;

        // color
        size_type      color_;

        // index of this element x_i in original sparse matrix
        size_type      prev_element_index_;

        // topological index (Saad, p. 370 and GPU-Accelerated Preconditioned
        // Iterative Linear Solvers, p. 16)
        size_type      topological_index_;
    };

} // namespace internal_NS

} // namespace LinearSolverLibrary_NS

#pragma warning(default:4251)
