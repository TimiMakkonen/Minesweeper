#ifndef MINESWEEPER_RANDOM_H
#define MINESWEEPER_RANDOM_H

#include <random> // std::random_device, std::mt19937
#include <vector> // std::vector

#include <minesweeper/i_random.h>

namespace minesweeper {

class Random : public IRandom {

  private:
    std::random_device _rd;
    std::mt19937 _uRng;

  public:
    Random();

    void shuffleVector(std::vector<int>& vec) override;
};
} // namespace minesweeper

#endif // MINESWEEPER_RANDOM_H