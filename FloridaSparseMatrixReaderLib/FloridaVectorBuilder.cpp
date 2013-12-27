#include "pch.h"

#include "FloridaVectorBuilder.h"


using namespace EntityReader_NS;
using namespace LinAlg_NS;


void
FloridaVectorBuilder::init(LinAlg_NS::Vector::size_type dim) const {
    data_.reset(new Vector(dim));
    dim_ = dim;
}

void
FloridaVectorBuilder::insertVectorElement(LinAlg_NS::Vector::size_type row, double value) const {
#ifdef _DEBUG
    bool assert_cond = row < dim_ && bool(data_);
    BOOST_ASSERT_MSG(assert_cond, "FloridaVectorBuilder::insertVectorElement: pre-condition error");
#endif
    if (row >= dim_)
        throw std::range_error("FloridaVectorBuilder::insertVectorElement: row index out of bounds");

    if (!data_)
        throw std::range_error("FloridaVectorBuilder::insertVectorElement: init needs to be called first");

    (*data_)(row) = value;
}

FloridaVectorBuilder::result_t
FloridaVectorBuilder::result() const {
    return data_;
}
