/*
 * Name  : IndexMap
 * Path  : 
 * Use   : Encapsulates the row and column indices for which
 *         a MatrixMatrixMul object has non-zero entires.
 * Author: Sven Schmidt
 * Date  : 01/11/2014
 */
#pragma once


namespace LinAlg_NS {

    namespace internal {

        class IndexMap {
        public:
            using size_type = IMatrix2D::size_type;

        public:
            IndexMap(std::function<std::vector<size_type>(size_type)> const & callback, size_type size)
                :
                callback_(callback) {
                indices_.resize(size);
            }

            std::vector<size_type>
                getIndices(size_type index) const {
#ifdef _DEBUG
                    common_NS::reporting::checkUppderBound(index, indices_.size() - 1);
#endif
                    auto const & indices = indices_[index];
                    if (indices.empty())
                        indices_[index] = callback_(index);
                    return indices;
                }

        private:
            mutable std::vector<std::vector<size_type>>      indices_;
            std::function<std::vector<size_type>(size_type)> callback_;
        };

    } // namespace internal

} // namespace LinAlg_NS
