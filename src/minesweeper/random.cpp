#include <algorithm> // std::shuffle

#include <minesweeper/random.h>

namespace minesweeper {

Random::Random() : _uRng(_rd()) {}

void Random::shuffleVector(std::vector<int>& vec) { std::shuffle(vec.begin(), vec.end(), this->_uRng); }

} // namespace minesweeper
