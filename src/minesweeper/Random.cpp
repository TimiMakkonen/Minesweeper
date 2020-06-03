#include <minesweeper/Random.h>

namespace Minesweeper {

	Random::Random() {
		this->uRng.seed(rd());
	}

	void Random::shuffleVector(std::vector<int>& vec) {

		std::shuffle(vec.begin(), vec.end(), this->uRng);

	}

}
