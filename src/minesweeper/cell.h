#ifndef MINESWEEPER_CELL_H
#define MINESWEEPER_CELL_H

#include <iostream> // std::istream, std::ostream

#include <nlohmann/json.hpp> // nlohmann::json

namespace minesweeper {

	class Cell {
	private:

		bool _isVisible = false;
		bool _hasMine = false;
		bool _isMarked = false;
		int _numOfMinesAround = 0;

	public:

		bool isVisible() const;

		bool hasMine() const;

		bool isMarked() const;

		int numOfMinesAround() const;

		void incrNumOfMinesAround();

		void makeVisible();

		void putMine();

		void markCell();

		void unmarkCell();

		// save cell:
		std::ostream& serialise(std::ostream& outStream) const;
		nlohmann::json serialise() const;

		// load cell:
		std::istream& deserialise(std::istream& inStream);
		void deserialise(nlohmann::json& j);

	};

}

#endif // MINESWEEPER_CELL_H