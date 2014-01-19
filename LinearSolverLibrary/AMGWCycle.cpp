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
            levels.push_back(current_level);
            init(current_level + 1, max_level, levels);
            levels.push_back(current_level);
            if (current_level > 0) {
                init(current_level + 1, max_level, levels);
                levels.push_back(current_level);
            }
        }
    }

}


void
AMGWCycle::initialize() {

    max_depth_ = 3;

    std::vector<size_t> levels;
    init(0, max_depth_, levels);
// 
//     int direction = 1;
//     size_t up_level = max_depth_ - 2;
//     auto current_level = 0;
//     do {
//         levels.push_back(current_level);
// 
//         for (auto l : levels) {
//             std::cout << l << " " << std::endl;
//         }
//         std::cout << std::endl;
// 
//         if (current_level == max_depth_ - 1) {
//             levels.push_back(max_depth_);
//             levels.push_back(current_level);
//             levels.push_back(max_depth_);
//             levels.push_back(current_level);
//             current_level--;
// //            up_level--;
//             direction *= -1;
//             continue;
//         }
//         if (current_level == up_level) {
//             direction *= -1;
//             up_level--;
//         }
//         current_level += direction;
//     
//     } while (current_level > 0);
//     levels.push_back(current_level);

    for (auto l : levels) {
        std::cout << l << " " << std::endl;
    }
    std::cout << std::endl;

}

size_t
AMGWCycle::currentLevel() const {
    return 0;
}

size_t
AMGWCycle::setNextLevel() const {
    return 0;
}
