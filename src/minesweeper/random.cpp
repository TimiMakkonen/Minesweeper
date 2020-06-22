#include <algorithm> // std::shuffle

#include <minesweeper/random.h>

namespace minesweeper {

Random::Random() : uRng(rd()) {}

void Random::shuffleVector(std::vector<int>& vec) { std::shuffle(vec.begin(), vec.end(), this->uRng); }

} // namespace minesweeper
