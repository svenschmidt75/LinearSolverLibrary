#include "SparseMatrixBuilder.h"

#include "LinAlg/SparseMatrix2D.h"

#include <stdexcept>

#include <boost/assert.hpp>


using namespace LinAlg_NS;
using namespace EntityReader_NS;


void
SparseMatrixBuilder::init(LinAlg_NS::IMatrix2D::size_type dim) const {
    data_.reset(new SparseMatrix2D(dim));
    dim_ = dim;
}

void
SparseMatrixBuilder::insertMatrixElement(LinAlg_NS::IMatrix2D::size_type row, LinAlg_NS::IMatrix2D::size_type col, double value) const {
    bool assert_cond = row < dim_ && col < dim_ && bool(data_);
    BOOST_ASSERT_MSG(assert_cond, "SparseMatrixBuilder::insertMatrixElement: post-condition error");
    if (row >= dim_)
        throw std::range_error("SparseMatrixBuilder::insertMatrixElement: row index out of bounds");

    if (col >= dim_)
        throw std::range_error("SparseMatrixBuilder::insertMatrixElement: column index out of bounds");

    if (!data_)
        throw std::range_error("SparseMatrixBuilder::insertMatrixElement: init needs to be called first");

    (*data_)(row, col) = value;
}
