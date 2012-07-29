#include "helper.h"


namespace LinAlg_NS {

TransposeVector
helper::transpose(Vector const & vec) {
    TransposeVector tv(vec.size());
    tv.vector_.data_ = vec.data_;
    return tv;
}

Vector
helper::transpose(TransposeVector const & vec) {
    Vector tv(vec.vector_);
    return tv;

}

} // namespace LinAlg_NS
