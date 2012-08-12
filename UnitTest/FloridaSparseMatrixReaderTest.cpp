#include "FloridaSparseMatrixReaderTest.h"

#include "FloridaSparseMatrixReader/FloridaSparseMatrixReader.h"
#include "FloridaSparseMatrixReader/SparseMatrixBuilder.h"

#include <string>


using namespace EntityReader_NS;


void
FloridaSparseMatrixReaderTest::setUp() {}

void
FloridaSparseMatrixReaderTest::tearDown() {}

void
FloridaSparseMatrixReaderTest::readTest() {
    SparseMatrixBuilder builder;
    std::string filename = "\\Develop\\SparseMatrixData\\offshore\\offshore.mtx";
    FloridaSparseMatrixReader reader(filename, builder);
    reader.read();
}
