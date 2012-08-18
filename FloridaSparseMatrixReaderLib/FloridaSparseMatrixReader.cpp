#include "FloridaSparseMatrixReader.h"

#include "IMatrixBuilder.h"

#include "LinAlg/IMatrix2D.h"

#include <boost/tokenizer.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/progress.hpp>
#include <boost/lexical_cast.hpp>

#include <string>
#include <cstdint>


using namespace EntityReader_NS;


FloridaSparseMatrixReader::FloridaSparseMatrixReader(std::string const & filename, IMatrixBuilder & builder)
    :
    filename_(filename),
    builder_(builder) {}

bool
FloridaSparseMatrixReader::read() const {
    boost::iostreams::stream<boost::iostreams::file_source> file(filename_);
    std::string line;
    boost::char_separator<char> sep(" ");

    std::uint64_t line_number = 0;

    std::vector<std::string> token_arr;

    // skip all comments
    while (std::getline(file, line)) {
        typedef boost::tokenizer<boost::char_separator<char>> TokenizerType;

        line_number++;

        boost::trim(line);
        TokenizerType tokens(line, sep);

        token_arr.assign(tokens.begin(), tokens.end());

        if (!token_arr.empty()) {
            std::string const & t = *(token_arr.begin());

            if (t[0] != '%')
                break;
        }
    }

    BOOST_ASSERT_MSG(token_arr.size() == 3ul, "FloridaSparseMatrixReader::read: File format error");
// print line

    // read matrix dimension and number of non-zero elements
    LinAlg_NS::IMatrix2D::size_type dim = boost::lexical_cast<std::uint64_t>(token_arr[0]);
    LinAlg_NS::IMatrix2D::size_type nelem = boost::lexical_cast<std::uint64_t>(token_arr[2]);

    // initialize builder
    builder_.init(dim);


    boost::progress_display show_progress((unsigned long)nelem);

    while (std::getline(file, line)) {
        typedef boost::tokenizer<boost::char_separator<char>> TokenizerType;

        line_number++;
        ++show_progress;

        boost::trim(line);
        TokenizerType tokens(line, sep);

        token_arr.assign(tokens.begin(), tokens.end());

        BOOST_ASSERT_MSG(token_arr.size() == 3ul, "FloridaSparseMatrixReader::read: File format error");
// print line

        LinAlg_NS::IMatrix2D::size_type row = boost::lexical_cast<std::uint64_t>(token_arr[0]) - 1;
        LinAlg_NS::IMatrix2D::size_type col = boost::lexical_cast<std::uint64_t>(token_arr[1]) - 1;
        double value = boost::lexical_cast<double>(token_arr[2]);
        builder_.insertMatrixElement(row, col, value);
    }

    builder_.finalize();

    return true;
}
