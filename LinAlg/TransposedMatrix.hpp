/*
 * Name  : TransposedMatrix
 * Path  : 
 * Use   : Wrapper for non-transposed matrix to behave like
           a transposed matrix.
 * Author: Sven Schmidt
 * Date  : 03/10/2013
 */
#pragma once



// #pragma warning(disable:4251)
// #pragma warning(disable:4275)


namespace LinAlg_NS {

    template<typename T>
    class TransposedMatrix : public IMatrix2D {
    public:
        TransposedMatrix(T const & in) : matrix_(in) {}

        // enable move semantics
//         SparseMatrix2D(SparseMatrix2D && in);
//         SparseMatrix2D & operator=(SparseMatrix2D && in);

        // FROM IMatrix2D
        size_type rows() const {
            return matrix_.rows();
        }

        size_type cols() const {
            return matrix_.cols();
        }

        double operator()(size_type row, size_type col) const {
            // swap cols and rows
            return matrix_(col, row);
        }

        double &  operator()(size_type row, size_type col) {
            // do nothing as this matrix is immutable
            BOOST_ASSERT_MSG(false, "TransposedMatrix is immutable");
            throw std::runtime_error("TransposedMatrix is immutable");
        }

        void      solve(Vector const & b, Vector & x) const {

        }

        // Local methods
        void print() const {

        }

    private:
        // reference to non-transposed matrix
        T const & matrix_;
    };

} // namespace LinAlg_NS

// #pragma warning(default:4275)
// #pragma warning(default:4251)
