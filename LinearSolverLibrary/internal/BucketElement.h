/*
 * Name  : BucketElement
 * Path  : 
 * Use   : Represents a component x_i in a a sparse matrix
 * Author: Sven Schmidt
 * Date  : 09/22/2012
 */
#pragma once

#include "..\DeclSpec.h"

#include <cstdint>
#include <memory>


#pragma warning(disable:4251)


namespace LinearSolverLibrary_NS {

namespace internal_NS {

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS BucketElement {
    public:
        typedef std::shared_ptr<BucketElement> Ptr;
        typedef std::uint64_t size_type;

    private:
        typedef std::set<Ptr> DependencyType;

    public:
        typedef DependencyType::const_iterator const_iterator;
        typedef DependencyType::iterator iterator;

    public:
        BucketElement(size_type element_index);

        const_iterator  cbegin() const;
        const_iterator  cend() const;

        iterator        begin() const;
        iterator        end() const;

        void            dependsOn(Ptr element_index);
        const_iterator  findDependency(BucketElement const & element) const;

        void            index(size_type index);
        size_type       index() const;

        size_type       prevIndex() const;

    private:
        // dependencies of this x_i to other x_j
        DependencyType dependencies_;

        // index of this element x_i in original sparse matrix
        size_type      prev_element_index_;

        // index of this element x_i in new sparse matrix
        size_type      element_index_;
    };

} // namespace internal_NS

} // namespace LinearSolverLibrary_NS

#pragma warning(default:4251)
