#include "VectorBinaryReader.h"

#include <boost/assert.hpp>
#include <boost/filesystem.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/nvp.hpp>

#include <istream>


using namespace EntityReader_NS;


VectorBinaryReader::VectorBinaryReader(std::string const & filename)
    :
    filename_(filename),
    data_(0) {};

bool
VectorBinaryReader::read() const {
    std::ifstream file(filename_, std::ios_base::binary);
    BOOST_ASSERT_MSG(file.good(), "Error opening output file name");
    boost::archive::binary_iarchive oa(file); 
    oa >> BOOST_SERIALIZATION_NVP(data_);
    file.close();

    return file.good();
}

LinAlg_NS::Vector const
VectorBinaryReader::get() const {
    return data_;
}
