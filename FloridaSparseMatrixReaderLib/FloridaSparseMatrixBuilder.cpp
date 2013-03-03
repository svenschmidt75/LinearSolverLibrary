#include "pch.h"

#include "FloridaSparseMatrixBuilder.h"
#include "internal/SymmetryStrategyFactory.h"

#include "LinAlg/SparseMatrix2D.h"

#include "common/reporting.h"


using namespace LinAlg_NS;
using namespace EntityReader_NS;


void
FloridaSparseMatrixBuilder::init(LinAlg_NS::IMatrix2D::size_type dim, std::string const & symmetry_strategy) const {
    data_.reset(new SparseMatrix2D(dim));
    dim_ = dim;

    // create symmetry strategy
    // Note: We don't have to explicitly move here, because SymmetryStrategyFactory::create
    // returns a tmp, hence the compiler will automatically chose the move assignment operator
    // on symmetry_strategy_.
    symmetry_strategy_ = SymmetryStrategyFactory::create(symmetry_strategy, *data_);
}

void
FloridaSparseMatrixBuilder::insertMatrixElement(LinAlg_NS::IMatrix2D::size_type row, LinAlg_NS::IMatrix2D::size_type col, double value) const {
#ifdef _DEBUG
    bool assert_cond = row < dim_ && col < dim_ && bool(data_);
    BOOST_ASSERT_MSG(assert_cond, "SparseMatrixBuilder::insertMatrixElement: pre-condition error");
#endif
    if (row >= dim_)
        throw std::range_error("SparseMatrixBuilder::insertMatrixElement: row index out of bounds");

    if (col >= dim_)
        throw std::range_error("SparseMatrixBuilder::insertMatrixElement: column index out of bounds");

    if (!data_)
        throw std::range_error("SparseMatrixBuilder::insertMatrixElement: init needs to be called first");

    symmetry_strategy_->insert(row, col, value);
}

void 
FloridaSparseMatrixBuilder::finalize() const {
#ifdef DEBUG
    bool assert_cond = bool(data_);
    BOOST_ASSERT_MSG(assert_cond, "SparseMatrixBuilder::finalize: pre-condition error");
#endif
    if (!data_)
        throw std::range_error("SparseMatrixBuilder::finalize: sparse matrix uninitialized");

    data_->finalize();

    // check matrix
    bool symmetry_strategy_check = symmetry_strategy_->check();
    BOOST_ASSERT_MSG(symmetry_strategy_check, "FloridaSparseMatrixBuilder::finalize: Matrix error");
    if (!symmetry_strategy_check) {
        boost::format format = boost::format("FloridaSparseMatrixBuilder::finalize: Matrix error!\n");
        common_NS::reporting::error(format.str());
        throw std::runtime_error(format.str());
    }
}

FloridaSparseMatrixBuilder::result_t
FloridaSparseMatrixBuilder::result() const {
    return data_;
}
