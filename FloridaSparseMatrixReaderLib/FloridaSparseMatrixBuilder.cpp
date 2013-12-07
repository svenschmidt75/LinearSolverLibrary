#include "pch.h"

#include "FloridaSparseMatrixBuilder.h"
#include "internal/SymmetryStrategyFactory.h"

#include "LinAlg/SparseMatrix2D.h"

#include "common/reporting.h"


using namespace LinAlg_NS;
using namespace EntityReader_NS;


void
FloridaSparseMatrixBuilder::init(LinAlg_NS::IMatrix2D::size_type rows, LinAlg_NS::IMatrix2D::size_type cols, std::string const & symmetry_strategy) const {
    data_.reset(new SparseMatrix2D(rows, cols));
    rows_ = rows;
    cols_ = cols;

    // create symmetry strategy
    // Note: We don't have to explicitly move here, because SymmetryStrategyFactory::create
    // returns a tmp, hence the compiler will automatically chose the move assignment operator
    // on symmetry_strategy_.
    symmetry_strategy_ = SymmetryStrategyFactory::create(symmetry_strategy, *data_);
}

void
FloridaSparseMatrixBuilder::insertMatrixElement(LinAlg_NS::IMatrix2D::size_type row, LinAlg_NS::IMatrix2D::size_type col, double value) const {
#ifdef _DEBUG
    bool assert_cond = row < rows_ && col < cols_ && bool(data_);
    BOOST_ASSERT_MSG(assert_cond, "FloridaSparseMatrixBuilder::insertMatrixElement: pre-condition error");
#endif
    if (row >= rows_)
        throw std::range_error("FloridaSparseMatrixBuilder::insertMatrixElement: row index out of bounds");

    if (col >= cols_)
        throw std::range_error("FloridaSparseMatrixBuilder::insertMatrixElement: column index out of bounds");

    if (!data_)
        throw std::range_error("FloridaSparseMatrixBuilder::insertMatrixElement: init needs to be called first");

    symmetry_strategy_->insert(row, col, value);
}

namespace {

    bool hasMatrixZeroDiagonalElements(SparseMatrix2D const & m) {
        for (IMatrix2D::size_type row = 0; row < m.rows(); ++row) {
            auto value = m(row, row);
            if (!value)
                return true;
        }
        return false;
    }

}

void 
FloridaSparseMatrixBuilder::finalize() const {
#ifdef DEBUG
    bool assert_cond = bool(data_);
    BOOST_ASSERT_MSG(assert_cond, "FloridaSparseMatrixBuilder::finalize: pre-condition error");
#endif
    if (!data_)
        throw std::range_error("FloridaSparseMatrixBuilder::finalize: sparse matrix uninitialized");

    data_->finalize();

    // check matrix
//     bool symmetry_strategy_check = symmetry_strategy_->check();
//     common_NS::reporting::checkConditional(symmetry_strategy_check, "FloridaSparseMatrixBuilder::finalize: Matrix error");

    common_NS::reporting::checkConditional(hasMatrixZeroDiagonalElements(*data_) == false, "FloridaSparseMatrixBuilder::finalize: Matrix contains zero diagonal elements!");
}

FloridaSparseMatrixBuilder::result_t
FloridaSparseMatrixBuilder::result() const {
    return data_;
}
