#include "FloridaSparseMatrixReaderTest.h"

#include "FloridaSparseMatrixReaderLib/FloridaSparseMatrixReader.h"
#include "FloridaSparseMatrixReaderLib/SparseMatrixBuilder.h"

#include <string>


using namespace EntityReader_NS;


void
FloridaSparseMatrixReaderTest::setUp() {}

void
FloridaSparseMatrixReaderTest::tearDown() {}

void
FloridaSparseMatrixReaderTest::readTest() {
    SparseMatrixBuilder builder;
    std::string filename = "\\Develop\\SparseMatrixData\\sts4098\\sts4098.mtx";
    FloridaSparseMatrixReader reader(filename, builder);
    bool success = reader.read();

    // assert #elements
    // dim
    // a couple of non-zero entries
}
