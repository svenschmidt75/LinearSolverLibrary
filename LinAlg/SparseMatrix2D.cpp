#include "SparseMatrix2D.h"

#include "Vector.h"

#include <iostream>
#include <iomanip>
#include <stdexcept>

#include <boost/mpl/bool_fwd.hpp>

#include <boost/assert.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/nvp.hpp>


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
    ncols_(0),
    finalized_(false) {}

SparseMatrix2D::SparseMatrix2D(size_type ncols)
    :
    ncols_(ncols),
    finalized_(false) {}

SparseMatrix2D::SparseMatrix2D(SparseMatrix2D const & in)
    :
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
    ncols_     = in.ncols_;
    data_      = in.data_;
    finalized_ = in.finalized_;
    elements_  = in.elements_;
    columns_   = in.columns_;
    nelements_ = in.nelements_;
}

SparseMatrix2D::size_type
SparseMatrix2D::rows() const {
    return ncols_;
}

SparseMatrix2D::size_type
SparseMatrix2D::cols() const {
    return ncols_;
}

double
SparseMatrix2D::operator()(SparseMatrix2D::size_type i, SparseMatrix2D::size_type j) const {
    // i: x, j: y

    if (finalized_) {
        // Number of non-zero columns for this row
        size_type ncol = nelements_[i + 1] - nelements_[i];
        size_type offset = nelements_[i];

        // column has zero elements
        if (!ncol)
            return 0.0;

        double value = 0.0;

        // check all columns for elements in row i
        for (size_type col = 0; col < ncol; ++col) {
            if (columns_[offset + col] < j)
                continue;

            if (columns_[offset + col] > j)
                break;

            value = elements_[offset + col];
            break;
        }

        return value;
    }

    // if not yet finalized
    Col_t & col = data_[i];
    return col[j];
}

double &
SparseMatrix2D::operator()(SparseMatrix2D::size_type i, SparseMatrix2D::size_type j) {
    // i: x, j: y

    if (finalized_)
        throw std::exception("SparseMatrix2D::operator(): Matrix already finalized");

    Col_t & col = data_[i];
    return col[j];
}

void
SparseMatrix2D::solve(Vector const & b, Vector & x) const {
    /* compute A x = b */
    if (!finalized_)
        throw std::exception("SparseMatrix2D::solve(): Matrix not yet finalized");

    bool assert_cond = b.size() == ncols_ && b.size() == x.size();
    BOOST_ASSERT_MSG(assert_cond, "Index range error");
    if (!assert_cond)
        throw std::out_of_range("SparseMatrix2D::solve(): Out of range error");

    size_type nrows = nelements_.size() - 1;

    // All rows
    for (int row = 0; row < nrows; ++row) {
        // Number of non-zero columns for this row
        size_type ncol = nelements_[row + 1] - nelements_[row];
        size_type offset = nelements_[row];

        double tmp = 0;

        // all non-zero columns
        for (int icol = 0; icol < ncol; ++icol) {
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

    Row_t::const_iterator row_it(data_.begin());
    Row_t::const_iterator row_it_end(data_.end());

    // all rows
    for (; row_it != row_it_end; ++row_it) {
//        int row = (*row_it).first;

        Col_t const & col = (*row_it).second;

        // all columns
        Col_t::const_iterator col_it(col.begin());
        Col_t::const_iterator col_it_end(col.end());

        for (; col_it != col_it_end; ++col_it) {
            size_type col = (*col_it).first;
            double value = (*col_it).second;

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
}

void
SparseMatrix2D::print() const {
    // number of rows
    size_type nrows = nelements_.size() - 1;

    std::cout << std::endl;

    for (size_type row = 0; row < nrows; ++row) {
        // Number of non-zero columns for this row
        size_type ncol = nelements_[row + 1] - nelements_[row];
        size_type offset = nelements_[row];

        size_type icol = 0;
        for (size_type col = 0; col < nrows; ++col) {
            // leading zeros
            if (columns_[offset + icol] > col) {
                std::cout << std::setw(8) << 0 << " ";
                continue;
            }

            double a_ij = elements_[offset + icol];
            std::cout << std::setw(8) << a_ij << " ";

            ++icol;

            if (icol == ncol) {
                // trailing zeros
                size_type diff = nrows - col;

                while(diff--)
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
