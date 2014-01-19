#include "pch.h"

#include "AMGWCycle.h"

#include <memory>


using namespace LinearSolverLibrary_NS;


AMGWCycle::AMGWCycle(short gamma, size_t max_depth)
    :
    gamma_{gamma},
    max_depth_{max_depth} {

    initialize();
}

namespace {

    void
    init(size_t current_level, size_t max_level, std::vector<size_t> & levels) {

        for (auto l : levels) {
            std::cout << l << " " << std::endl;
        }
        std::cout << std::endl;

        if (current_level > max_level)
            return;

        if (current_level == max_level)
            levels.push_back(current_level);

        if (current_level < max_level) {
            int gamma = 2;
            for (int i = 0; i < gamma; ++i) {
                levels.push_back(current_level);
                init(current_level + 1, max_level, levels);
            }
            levels.push_back(current_level);
//             levels.push_back(current_level);
//             init(current_level + 1, max_level, levels);
//             levels.push_back(current_level);
//             if (current_level > 0) {
//                 init(current_level + 1, max_level, levels);
//                 levels.push_back(current_level);
//             }
        }
    }

}


void
AMGWCycle::initialize() {

    max_depth_ = 3;

    levels_.push_back(0);
    init(1, max_depth_, levels_);
    levels_.push_back(0);


    for (auto l : levels_) {
        std::cout << l << " " << std::endl;
    }
    std::cout << std::endl;

    print();
}

size_t
AMGWCycle::currentLevel() const {
    return 0;
}

size_t
AMGWCycle::setNextLevel() const {
    return 0;
}

void
AMGWCycle::print() const {
    std::cout << std::endl;
    for (auto row = 0; row <= max_depth_; ++row) {
        for (auto column = 0; column < levels_.size(); ++column) {
            auto level = levels_[column];
            if (level == row)
                std::cout << level;
            else
                std::cout << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;


    {
        std::vector<char> buffer;
        buffer.resize(levels_.size() * (max_depth_ + 1) + max_depth_ + 1);

        std::cout << std::endl;

        size_t previous_level = 0;
        for (auto column = 0; column < levels_.size(); ++column) {
            for (auto row = 0; row <= max_depth_; ++row) {
                auto index = row * (levels_.size() + 1) + column;
                auto level = levels_[column];
                if (level == row) {
                    char s;
                    if (previous_level <= level) {
                        s = '\\';
                    }
                    else {
                        s = '/';
                    }
                    buffer[index] = s;
                    previous_level = level;
                }
                else
                    buffer[index] = ' ';
            }
        }

        for (auto row = 0; row <= max_depth_; ++row) {
            auto index = row * (levels_.size() + 1) + levels_.size();
            buffer[index] = '\n';
        }

        std::string output(std::cbegin(buffer), std::cend(buffer));
        std::cout << output;
        std::cout << std::endl;
    }
}