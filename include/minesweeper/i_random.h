#ifndef MINESWEEPER_I_RANDOM_H
#define MINESWEEPER_I_RANDOM_H

#include <vector> // std::vector

namespace minesweeper {

class IRandom {

  public:
    virtual void shuffleVector(std::vector<int>& vec) = 0;

    virtual ~IRandom() = default;

    IRandom() = default;
    IRandom(const IRandom& other) = default;
    IRandom& operator=(const IRandom& other) = default;
    IRandom(IRandom&& other) = default;
    IRandom& operator=(IRandom&& other) = default;
};
} // namespace minesweeper

#endif // MINESWEEPER_I_RANDOM_H