/*
* Name  : AMGWCycle
* Path  :
* Use   : 
* Author: Sven Schmidt
* Date  : 01/19/2014
*/
#pragma once

#include "DeclSpec.h"


#pragma warning(disable:4275)
#pragma warning(disable:4251)

namespace LinearSolverLibrary_NS {

    class IState;
    class DescendState;

    class LINEARSOLVERLIBRARY_DECL_SYMBOLS AMGWCycle {

        friend class DescendState;

    public:
        AMGWCycle(short gamma, size_t max_depth);

        size_t currentLevel() const;
        size_t setNextLevel() const;

    private:
        using IStatePtr = std::unique_ptr<IState, std::default_delete<IState>>;

    private:
        void initialize();
        void setState(IStatePtr state);

    private:
        size_t    max_depth_;
        short     gamma_;
        short     gamma2_;
        size_t    levels_to_ascend_;
        IStatePtr state_;
    };



    class IState {
    public:
        virtual ~IState() {}

        virtual size_t currentLevel() const = 0;
        virtual size_t setNextLevel() = 0;
    };

    class DescendState : public IState {
    public:
        DescendState(AMGWCycle & cycle, size_t current_level)
            :
            current_level_{current_level},
            cycle_{cycle} {}

        DescendState(const DescendState &) = delete;
        DescendState & operator=(DescendState const &) = delete;

        size_t currentLevel() const {
            return current_level_;
        }

        size_t setNextLevel() {
            if (current_level_ < cycle_.max_depth_)
                current_level_++;
            else {
                // lowest level reached, move up
                if (cycle_.gamma2_ < 2) {
                    auto ascend_state = std::make_unique<AscendState>(cycle_, 0);
                }
            }
        }

    private:
        size_t      current_level_;
        AMGWCycle & cycle_;
    };

} // LinearSolverLibrary_NS

#pragma warning(default:4251)
#pragma warning(default:4275)
