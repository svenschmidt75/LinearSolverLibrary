#include "SparseMatrixBinaryReader.h"

#include <boost/assert.hpp>
#include <boost/filesystem.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/nvp.hpp>

#include <istream>


using namespace EntityReader_NS;


SparseMatrixBinaryReader::SparseMatrixBinaryReader(std::string const & filename)
    :
    filename_(filename) {};

bool
SparseMatrixBinaryReader::read() const {
    std::ifstream file(filename_, std::ios_base::binary);
    BOOST_ASSERT_MSG(file.good(), "Error opening output file name");
    boost::archive::binary_iarchive oa(file); 
    oa >> BOOST_SERIALIZATION_NVP(m_);
    file.close();

    return file.good();
}

LinAlg_NS::SparseMatrix2D
SparseMatrixBinaryReader::get() const {
    return m_;
}
