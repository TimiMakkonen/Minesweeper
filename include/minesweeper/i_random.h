#ifndef MINESWEEPER_I_RANDOM_H
#define MINESWEEPER_I_RANDOM_H

#include <vector>

namespace minesweeper {

	class IRandom {

	public:

		virtual void shuffleVector(std::vector<int>& vec) = 0;

	};
}

#endif // MINESWEEPER_I_RANDOM_H