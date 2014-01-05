/*
 * Name  : parallel
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 12/28/2012
 */
#pragma once

#define PARALLEL
//#undef PARALLEL


namespace common_NS {

    template<typename T>
    std::tuple<T, T>
    getChunkStartEndIndex(T total_size, T nchunks, T current_start_index) {
        // Note: current_start_index MUST be a multiple of chunk_size.
        // This method returns nonsense otherwise!!!
        auto chunk_size = total_size / nchunks;
        auto diff = total_size % nchunks;

        auto chunk_index = current_start_index / chunk_size;
        auto start_index = current_start_index;
        auto end_size = current_start_index + chunk_size;

        if (diff) {
            if (chunk_index < diff) {
                start_index += chunk_index;
                end_size = start_index + chunk_size + 1;
            }
            else {
                start_index += diff;
                end_size = start_index + chunk_size;
            }
        }
        return std::make_tuple(start_index, end_size);
    }

    template<typename T1, typename T2>
    auto
    getAdjustedSize(T1 total_size, T2 nchunks) -> decltype(T1() - T2()) {
//        auto chunk_size = total_size / nchunks;
        auto diff = total_size % nchunks;
        return total_size - diff;
    }

} // namespace common_NS
