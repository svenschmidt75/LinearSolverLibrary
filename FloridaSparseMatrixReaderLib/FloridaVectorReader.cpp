#include "FloridaVectorReader.h"

#include "IVectorBuilder.h"

#include "LinAlg/Vector.h"

#include "common/reporting.h"

#include <boost/tokenizer.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/progress.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

#include <string>
#include <cstdint>


using namespace EntityReader_NS;


FloridaVectorReader::FloridaVectorReader(std::string const & filename, IVectorBuilder<LinAlg_NS::Vector> & builder)
    :
    filename_(filename),
    builder_(builder) {}

bool
FloridaVectorReader::read() const {
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

    bool cond = token_arr.size() == 2ul || token_arr.size() == 3ul;
    BOOST_ASSERT_MSG(cond, "FloridaVectorReader::read: File format error");
    if (!cond) {
        boost::format format = boost::format("FloridaVectorReader::read: File %1%, line %2%: Data format error!\n") % line_number % filename_;
        common_NS::reporting::error(format.str());
        return false;
    }

    // print line

    // read vector dimension
    LinAlg_NS::Vector::size_type dim = boost::lexical_cast<std::uint64_t>(token_arr[0]);

    // read cols
    LinAlg_NS::Vector::size_type cols = boost::lexical_cast<std::uint64_t>(token_arr[1]);
    if (cols != 1) {
        boost::format format = boost::format("FloridaVectorReader::read: File %1%, line %2%: Number of columns not 1!\n") % line_number % filename_;
        common_NS::reporting::error(format.str());
        return false;
    }

    // #of non-zero elements?
    LinAlg_NS::Vector::size_type nelem = dim;
    LinAlg_NS::Vector::size_type nelem_act = 0;
    if (token_arr.size() > 2)
        nelem = boost::lexical_cast<std::uint64_t>(token_arr[2]);

    // initialize builder
    builder_.init(dim);

    LinAlg_NS::IMatrix2D::size_type row = 0;


    boost::progress_display show_progress((unsigned long)nelem);

    while (std::getline(file, line)) {
        typedef boost::tokenizer<boost::char_separator<char>> TokenizerType;

        line_number++;
        ++show_progress;

        boost::trim(line);
        TokenizerType tokens(line, sep);

        token_arr.assign(tokens.begin(), tokens.end());

        /* 
         * 1. row col value
         * 2. row value
         * 3. value
         */
        cond = token_arr.size() >= 1 && token_arr.size() <= 3ul;
        BOOST_ASSERT_MSG(cond, "FloridaVectorReader::read: File format error");
        if (!cond) {
            boost::format format = boost::format("FloridaVectorReader::read: File %1%, line %2%: Data format error!\n") % line_number % filename_;
            common_NS::reporting::error(format.str());
            return false;
        }

        double value = 0;

        if (token_arr.size() == 1) {
            // read value
            value = boost::lexical_cast<double>(token_arr[0]);
        } else if (token_arr.size() == 2) {
            // read row - value
            row = boost::lexical_cast<std::uint64_t>(token_arr[0]) - 1;
            value = boost::lexical_cast<double>(token_arr[1]);
        } else {
            // read row - col - value
            row = boost::lexical_cast<std::uint64_t>(token_arr[0]) - 1;
            value = boost::lexical_cast<double>(token_arr[2]);
        }

        nelem_act++;

        builder_.insertVectorElement(row, value);
    }

    cond = nelem == nelem_act;
    BOOST_ASSERT_MSG(cond, "FloridaVectorReader::read: File format error");
    if (!cond) {
        boost::format format = boost::format("FloridaVectorReader::read: File %1%, line %2%: Data format error!\n") % line_number % filename_;
        common_NS::reporting::error(format.str());
        return false;
    }

    return true;
}
