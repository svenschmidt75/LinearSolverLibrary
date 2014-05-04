#include "pch.h"

#include "VectorMath.h"

#include "Vector.h"
#include "IMatrix2D.h"


namespace LinAlg_NS {

double
VectorMath::dotProduct(Vector const & lhs, Vector const & rhs) {
#ifdef PARALLEL
    return nonChunkedParallelDotProduct(lhs, rhs);
//    return chunkedParallelDotProduct(lhs, rhs);
#else
    return dotProductSerial(lhs, rhs);
#endif
}

double
VectorMath::dotProductSerial(Vector const & lhs, Vector const & rhs) {
#ifdef _DEBUG
    common_NS::reporting::checkConditional(lhs.size() == rhs.size(), "VectorMath::dotProduct: Size mismatch");
#endif
    double tmp = 0.0;
    for (Vector::size_type i = 0; i < lhs.size(); ++i) {
        tmp += lhs(i) * rhs(i);
    }
    return tmp;
}

double
VectorMath::chunkedParallelDotProduct(Vector const & v1, Vector const & v2) {
    using size_type = IMatrix2D::size_type;
    double result = 0;
    concurrency::combinable<double> part_sums([]{
        return 0;
    });
    auto vector_size = v1.size();
    size_type numberOfProcessors = std::thread::hardware_concurrency();
    if (vector_size < numberOfProcessors)
        numberOfProcessors = vector_size;
    size_type chunk_size = vector_size / numberOfProcessors;
#if _DEBUG
    common_NS::reporting::checkConditional(chunk_size, "chunk_size cannot be null");
#endif
    auto size = common_NS::getAdjustedSize(vector_size, numberOfProcessors);
    concurrency::parallel_for(size_type{0}, size, chunk_size, [&part_sums, &v1, &v2, vector_size, numberOfProcessors](size_type index) {
        size_type start_row, end_size;
        std::tie(start_row, end_size) = common_NS::getChunkStartEndIndex(vector_size, size_type{numberOfProcessors}, index);
        double part_result = 0;
        for (size_type i = start_row; i < end_size; ++i) {
            double tmp = v1(i) * v2(i);
            part_result += tmp;
        }
        part_sums.local() += part_result;
    }, concurrency::static_partitioner());
    result = part_sums.combine(std::plus<double>());
    return result;
}

double
VectorMath::nonChunkedParallelDotProduct(Vector const & v1, Vector const & v2) {
    using size_type = IMatrix2D::size_type;
    double result = 0;
    concurrency::combinable<double> part_sums([]{
        return 0;
    });
    concurrency::parallel_for(size_type{0}, v1.size(), [&part_sums, &v1, &v2](size_type row_index) {
        double tmp = v1(row_index) * v2(row_index);
        part_sums.local() += tmp;
    });
    result = part_sums.combine(std::plus<double>());
    return result;
}

double
VectorMath::norm(Vector const & v) {
    return std::sqrt(dotProduct(v, v));
}

double
VectorMath::LinfError(Vector const & lhs, Vector const & rhs) {
    common_NS::reporting::checkConditional(lhs.size() == rhs.size(), "VectorMath::LinfError: Incompatible vectors");
    double abs = 0;
    for (Vector::size_type index{0}; index < lhs.size(); ++index) {
        double delta = std::fabs(lhs(index) - rhs(index));
        if (delta > abs)
            abs = delta;
    }
    return abs;
}

double
VectorMath::L2Error(Vector const & lhs, Vector const & rhs) {
    common_NS::reporting::checkConditional(lhs.size() == rhs.size(), "VectorMath::LinfError: Incompatible vectors");
    double sum_squares = 0;
    for (Vector::size_type index{0}; index < lhs.size(); ++index) {
        double delta = lhs(index) - rhs(index);
        sum_squares += delta * delta;
    }
    sum_squares = std::sqrt(sum_squares);
    return sum_squares;
}

} // namespace LinAlg_NS
