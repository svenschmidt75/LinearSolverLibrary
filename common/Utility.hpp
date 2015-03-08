/*
 * Name  : Utility
 * Path  :
 * Use   : Utility methods
 * Author: Sven Schmidt
 * Date  : 03/08/2015
 */
#pragma once



namespace common_NS {

    template<typename U, typename T>
    std::unique_ptr<U>
    convert(std::unique_ptr<T> & ptr) {
        return std::unique_ptr<U>(std::move(ptr));
    }

} // namespace common_NS

