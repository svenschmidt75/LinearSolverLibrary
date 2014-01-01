#include "pch.h"

#include "SparseMatrix2D.h"
#include "Vector.h"


/* The following specializations are needed for serializing
 * in the xml file format. Not sure why...
 */
// namespace boost {
//     namespace serialization {
// 
//         template<>
//         struct is_wrapper<const LinAlg_NS::IMatrix2D::size_type> : public boost::mpl::true_ {
//         };
// 
//         template<>
//         struct is_wrapper<LinAlg_NS::IMatrix2D::size_type> : public boost::mpl::true_ {
//         };
// 
//         template<>
//         struct is_wrapper<const LinAlg_NS::SparseMatrix2D::Row_t> : public boost::mpl::true_ {
//         };
// 
//         template<>
//         struct is_wrapper<LinAlg_NS::SparseMatrix2D::Row_t> : public boost::mpl::true_ {
//         };
// 
//         template<>
//         struct is_wrapper<const bool> : public boost::mpl::true_ {
//         };
// 
//         template<>
//         struct is_wrapper<bool> : public boost::mpl::true_ {
//         };
// 
//         template<>
//         struct is_wrapper<const std::vector<double>> : public boost::mpl::true_ {
//         };
// 
//         template<>
//         struct is_wrapper<std::vector<double>> : public boost::mpl::true_ {
//         };
// 
//         template<>
//         struct is_wrapper<const std::vector<LinAlg_NS::IMatrix2D::size_type>> : public boost::mpl::true_ {
//         };
// 
//         template<>
//         struct is_wrapper<std::vector<LinAlg_NS::IMatrix2D::size_type>> : public boost::mpl::true_ {
//         };
//         
//     }
// }


namespace LinAlg_NS {


SparseMatrix2D::SparseMatrix2D()
    :
    SparseMatrix2D(0, 0) {}

SparseMatrix2D::SparseMatrix2D(size_type dimension)
    :
    SparseMatrix2D(dimension, dimension) {}

SparseMatrix2D::SparseMatrix2D(size_type rows, size_type columns)
    :
    nrows_(rows),
    ncols_(columns),
    finalized_(false) {}

SparseMatrix2D::SparseMatrix2D(SparseMatrix2D const & in)
    :
    nrows_(in.nrows_),
    ncols_(in.ncols_),
    data_(in.data_),
    finalized_(in.finalized_),
    elements_(in.elements_),
    columns_(in.columns_),
    rows_(in.rows_),
    columns_offset_(in.columns_offset_),
    rows_offset_(in.rows_offset_) {}

SparseMatrix2D &
SparseMatrix2D::operator=(SparseMatrix2D const & in) {
    /* Strong exception safety: First, assign in to a temp
     * object. This way, in case an exception occurs, "this"
     * is not messed up.
     * We then swap temp with "this".
     */

    // copy-construction is exception safe
    SparseMatrix2D temp(in);
    swap(temp);
    return *this;
}

SparseMatrix2D::SparseMatrix2D(SparseMatrix2D && in)
    :
    nrows_(in.nrows_),              // value semantics: no move necessary
    ncols_(in.ncols_),              // value semantics: no move necessary
    data_(std::move(in.data_)),
    finalized_(in.finalized_),
    elements_(std::move(in.elements_)),
    columns_(std::move(in.columns_)),
    rows_(std::move(in.rows_)),
    columns_offset_(std::move(in.columns_offset_)),
    rows_offset_(std::move(in.rows_offset_)) {}

SparseMatrix2D &
SparseMatrix2D::operator=(SparseMatrix2D && in) {
    /* Strong exception safety: First, assign in to a temp
     * object. This way, in case an exception occurs, "this"
     * is not messed up.
     * We then swap temp with "this".
     */

    // force move copy-construction, is exception safe
    SparseMatrix2D temp(std::move(in));
    swap(temp);
    return *this;
}

void
SparseMatrix2D::swap(SparseMatrix2D const & in) {
    // to enable the safe exception guarantee
    nrows_          = in.nrows_;
    ncols_          = in.ncols_;
    data_           = in.data_;
    finalized_      = in.finalized_;
    elements_       = in.elements_;
    columns_        = in.columns_;
    rows_           = in.rows_;
    columns_offset_ = in.columns_offset_;
    rows_offset_    = in.rows_offset_;
}

SparseMatrix2D::size_type
SparseMatrix2D::rows() const {
    return nrows_;
}

SparseMatrix2D::size_type
SparseMatrix2D::cols() const {
    return ncols_;
}

double
SparseMatrix2D::operator()(SparseMatrix2D::size_type row, SparseMatrix2D::size_type column) const {
#ifdef _DEBUG
    common_NS::reporting::checkUppderBound(row, rows() - 1);
    common_NS::reporting::checkUppderBound(column, cols() - 1);
#endif
    if (finalized_) {
        // Number of non-zero columns for this row
        size_type ncol = columns_offset_[row + 1] - columns_offset_[row];
        if (!ncol) return 0.0;
        double value = 0.0;
        size_type offset = columns_offset_[row];

        // check all columns for elements in row 'row'
        for (size_type icol = 0; icol < ncol; ++icol) {
            size_type col = columns_[offset + icol];
            if (col < column)
                continue;
            if (col > column)
                break;
            value = elements_[offset + icol];
            break;
        }
        return value;
    }
    // if not yet finalized
    Col_t & col = data_[row];
    return col[column];
}

double &
SparseMatrix2D::operator()(SparseMatrix2D::size_type row, SparseMatrix2D::size_type column) {
    // Note: This method is NOT thread save!
#ifdef _DEBUG
    common_NS::reporting::checkConditional(finalized_ == false, "SparseMatrix2D::operator(): Matrix already finalized");
    common_NS::reporting::checkUppderBound(row, rows() - 1);
    common_NS::reporting::checkUppderBound(column, cols() - 1);
#endif
    row_to_column_map_[row].insert(column);
    column_to_row_map_[column].insert(row);
    Col_t & col = data_[row];
    return col[column];
}

void
SparseMatrix2D::solve(Vector const & b, Vector & x) const {
    /* compute A x = b */
#ifdef _DEBUG
    common_NS::reporting::checkConditional(finalized_, "SparseMatrix2D::solve(): Matrix not yet finalized");
    common_NS::reporting::checkConditional(b.size() == ncols_ && b.size() == x.size());
#endif

    size_type nrows = columns_offset_.size() - 1;

    for (size_type row = 0; row < nrows; ++row) {
        // Number of non-zero columns for this row
        size_type ncol = columns_offset_[row + 1] - columns_offset_[row];
        size_type offset = columns_offset_[row];

        double tmp = 0;

        // all non-zero columns
        for (size_type icol = 0; icol < ncol; ++icol) {
            size_type col = columns_[offset + icol];
            double a_ij = elements_[offset + icol];
            tmp += (a_ij * b(col));
        }
        x(row) = tmp;
    }
}

void
SparseMatrix2D::finalizeColumnIndices() const {
    // Note: It is assumed that all the column indices
    // in row_to_column_map_[row] are sorted!
    size_type container_size = row_to_column_map_.size();
    columns_offset_.reserve(nrows_ + 1);
    columns_.reserve(container_size);
    columns_offset_.push_back(0);
    size_type column_offset{0};
    for (size_type row_index = 0; row_index < nrows_; ++row_index) {
        auto it = row_to_column_map_.find(row_index);
        if (it == std::cend(row_to_column_map_)) {
            auto previous_offset = columns_offset_[columns_offset_.size() - 1];
            columns_offset_.push_back(previous_offset);
            continue;
        }

        // *it == std::pair<size_type const, std::set<size_type>>

        size_type size = 0;
        for (auto it2 = std::cbegin(it->second); it2 != std::cend(it->second); ++it2) {
            auto column_index = *it2;
            if ((*this)(row_index, column_index)) {
                columns_.push_back(column_index);
                size++;
            }
        }
        column_offset += size;
        columns_offset_.push_back(column_offset);
    }
    row_to_column_map_.clear();
}

void
SparseMatrix2D::finalizeRowIndices() const {
    // Note: It is assumed that all the row indices
    // in column_to_row_map_[column] are sorted!
    size_type container_size = row_to_column_map_.size();
    rows_offset_.reserve(ncols_ + 1);
    rows_.reserve(container_size);
    rows_offset_.push_back(0);
    size_type row_offset{0};
    for (size_type column_index = 0; column_index < ncols_; ++column_index) {
        auto it = column_to_row_map_.find(column_index);
        if (it == std::cend(column_to_row_map_)) {
            auto previous_offset = rows_offset_[rows_offset_.size() - 1];
            rows_offset_.push_back(previous_offset);
            continue;
        }

        // *it == std::pair<size_type const, std::set<size_type>>

        size_type size = 0;
        for (auto it2 = std::cbegin(it->second); it2 != std::cend(it->second); ++it2) {
            auto row_index = *it2;
            if ((*this)(row_index, column_index)) {
                rows_.push_back(row_index);
                size++;
            }
        }
        row_offset += size;
        rows_offset_.push_back(row_offset);
    }
    column_to_row_map_.clear();
}

void
SparseMatrix2D::finalize() const {
    /* Convert to extended compressed sparse row storage format, eCSR.
     * All explicit 0 are dropped.
     */
    finalizeColumnIndices(); 
    finalizeRowIndices();

    for (auto const & row_item : data_) {
//        int row = (*row_it).first;
        Col_t const & col = row_item.second;

        for (auto const & col_item : col) {
//            size_type col(col_item.first);
            double value(col_item.second);
            if (!value)
                continue;
            elements_.push_back(value);
        }
    }

    // Matrix has been finalized
    finalized_ = true;

    // release old data
    data_.clear();
}

std::vector<SparseMatrix2D::size_type>
SparseMatrix2D::getNonZeroColumnIndicesForRow(size_type row) const {
#ifdef _DEBUG
    common_NS::reporting::checkUppderBound(row, rows() - 1);
#endif
    size_type column_offset = columns_offset_[row];
    size_type nelements = columns_offset_[row + 1] - column_offset;
    std::vector<size_type> column_indices;
    column_indices.reserve(nelements);
    for (auto i = 0; i < nelements; ++i) {
        auto column_index = columns_[column_offset + i];
        column_indices.push_back(column_index);
    }
    return column_indices;
}

std::vector<SparseMatrix2D::size_type>
SparseMatrix2D::getNonZeroRowIndicesForColumn(size_type column) const {
#ifdef _DEBUG
    common_NS::reporting::checkUppderBound(column, cols() - 1);
#endif
    size_type row_offset = rows_offset_[column];
    size_type nelements = rows_offset_[column + 1] - row_offset;
    std::vector<size_type> row_indices;
    row_indices.reserve(nelements);
    for (auto i = 0; i < nelements; ++i) {
        auto row_index = rows_[row_offset + i];
        row_indices.push_back(row_index);
    }
    return row_indices;
}

void
SparseMatrix2D::print() const {
    std::cout << std::endl;

    for (size_type row = 0; row < nrows_; ++row) {
        // Number of non-zero columns for this row
        size_type ncol = columns_offset_[row + 1] - columns_offset_[row];
        size_type offset = columns_offset_[row];

        size_type column = columns_[offset];
        size_type icol = 0;
        for (size_type col = 0; col < ncols_; ++col) {
            column = columns_[offset + icol];

            // print leading 0
            if (column > col) {
                std::cout << std::setw(8) << 0 << " ";
                continue;
            }

            common_NS::reporting::checkConditional(col == column);

            double a_ij = elements_[offset + icol];
            std::cout << std::setw(8) << a_ij << " ";

            ++icol;

            if (icol == ncol) {
                // trailing zeros
                size_type diff = ncols_ - col;

                while(--diff)
                    std::cout << std::setw(8) << 0 << " ";

                break;
            }
        }

        std::cout << std::endl;
    }
}

template<typename AR>
void
serialize_helper(AR & ar, SparseMatrix2D & m, const unsigned int /*version*/) {
    ar & BOOST_SERIALIZATION_NVP(m.nrows_);
    ar & BOOST_SERIALIZATION_NVP(m.ncols_);
    ar & BOOST_SERIALIZATION_NVP(m.data_);
    ar & BOOST_SERIALIZATION_NVP(m.finalized_);
    ar & BOOST_SERIALIZATION_NVP(m.elements_);
    ar & BOOST_SERIALIZATION_NVP(m.rows_);
    ar & BOOST_SERIALIZATION_NVP(m.columns_);
    ar & BOOST_SERIALIZATION_NVP(m.columns_offset_);
    ar & BOOST_SERIALIZATION_NVP(m.rows_offset_);
}

void
serialize(boost::archive::text_oarchive & ar, SparseMatrix2D & m, const unsigned int version) {
    serialize_helper(ar, m, version);
}

void
serialize(boost::archive::text_iarchive & ar, SparseMatrix2D & m, const unsigned int version) {
    serialize_helper(ar, m, version);
}

void
serialize(boost::archive::xml_oarchive & ar, SparseMatrix2D & m, const unsigned int version) {
    serialize_helper(ar, m, version);
}

void
serialize(boost::archive::xml_iarchive & ar, SparseMatrix2D & m, const unsigned int version) {
    serialize_helper(ar, m, version);
}

void
serialize(boost::archive::binary_oarchive & ar, SparseMatrix2D & m, const unsigned int version) {
    serialize_helper(ar, m, version);
}

void
serialize(boost::archive::binary_iarchive & ar, SparseMatrix2D & m, const unsigned int version) {
    serialize_helper(ar, m, version);
}

} // namespace LinAlg_NS
