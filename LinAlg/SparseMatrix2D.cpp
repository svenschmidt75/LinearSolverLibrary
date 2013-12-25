#include "pch.h"

#include "SparseMatrix2D.h"
#include "Vector.h"
#include "common/reporting.h"


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
    nelements_(in.nelements_) {}

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
    nelements_(std::move(in.nelements_)) {}

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
    nrows_     = in.nrows_;
    ncols_     = in.ncols_;
    data_      = in.data_;
    finalized_ = in.finalized_;
    elements_  = in.elements_;
    columns_   = in.columns_;
    nelements_ = in.nelements_;
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
        size_type ncol = nelements_[row + 1] - nelements_[row];
        if (!ncol) return 0.0;
        double value = 0.0;
        size_type offset = nelements_[row];

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

    size_type nrows = nelements_.size() - 1;

    for (size_type row = 0; row < nrows; ++row) {
        // Number of non-zero columns for this row
        size_type ncol = nelements_[row + 1] - nelements_[row];
        size_type offset = nelements_[row];

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
SparseMatrix2D::finalize() const {
    // convert to compressed row storage format
    size_type nelements = 0;
    size_type nelements_total = 0;

    Row_t::const_iterator row_it(std::cbegin(data_));
    Row_t::const_iterator row_it_end(std::cend(data_));

    // all rows
    for (; row_it != row_it_end; ++row_it) {
//        int row = (*row_it).first;
        Col_t const & col = (*row_it).second;

        // all columns
        Col_t::const_iterator col_it(std::cbegin(col));
        Col_t::const_iterator col_it_end(std::cend(col));

        for (; col_it != col_it_end; ++col_it) {
            size_type col = (*col_it).first;
            double value = (*col_it).second;
            if (!value)
                continue;

            elements_.push_back(value);
            columns_.push_back(col);

            // Number of non-zero elements
            nelements++;
        }
        nelements_.push_back(nelements_total);
        nelements_total += nelements;
        nelements = 0;
    }
    nelements_.push_back(nelements_total);

    // Matrix has been finalized
    finalized_ = true;

    // release old data
    data_.clear();
}

void
SparseMatrix2D::print() const {
    std::cout << std::endl;

    for (size_type row = 0; row < nrows_; ++row) {
        // Number of non-zero columns for this row
        size_type ncol = nelements_[row + 1] - nelements_[row];
        size_type offset = nelements_[row];

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
    ar & BOOST_SERIALIZATION_NVP(m.columns_);
    ar & BOOST_SERIALIZATION_NVP(m.nelements_);
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
