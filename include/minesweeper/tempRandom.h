#ifndef MINESWEEPER_RANDOM_H
#define MINESWEEPER_RANDOM_H

#include <random>
#include <vector>

#include <minesweeper/i_random.h>

namespace minesweeper {

	class Random : public IRandom {

	private:

		std::random_device rd;
		std::mt19937 uRng;


	public:

		Random();

		void shuffleVector(std::vector<int>& vec) final override;

	};
}

#endif // MINESWEEPER_RANDOM_H