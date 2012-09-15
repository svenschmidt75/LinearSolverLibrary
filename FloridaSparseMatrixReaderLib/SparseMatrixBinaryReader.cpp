#include "pch.h"

#include "SparseMatrixBinaryReader.h"


using namespace EntityReader_NS;


SparseMatrixBinaryReader::SparseMatrixBinaryReader(std::string const & filename)
    :
    filename_(filename) {};

bool
SparseMatrixBinaryReader::read() const {
    std::ifstream file(filename_, std::ios_base::binary);
    BOOST_ASSERT_MSG(file.good(), "Error opening output file name");
    boost::archive::binary_iarchive oa(file); 
    oa >> BOOST_SERIALIZATION_NVP(data_);
    file.close();

    return file.good();
}

LinAlg_NS::SparseMatrix2D const
SparseMatrixBinaryReader::get() const {
    return data_;
}
