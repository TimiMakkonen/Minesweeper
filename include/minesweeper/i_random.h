#ifndef MINESWEEPER_I_RANDOM_H
#define MINESWEEPER_I_RANDOM_H

#include <vector> // std::vector

namespace minesweeper {

class IRandom {

  public:
    virtual void shuffleVector(std::vector<int>& vec) = 0;

    virtual ~IRandom() {}
};
} // namespace minesweeper

#endif // MINESWEEPER_I_RANDOM_H