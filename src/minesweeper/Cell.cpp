#include <assert.h>

#include <minesweeper/cell.h>

namespace minesweeper {

	bool Cell::isVisible() const {

		return this->_isVisible;
	}



	bool Cell::hasMine() const {

		return this->_hasMine;
	}



	bool Cell::isMarked() const {

		return this->_isMarked;
	}



	int Cell::numOfMinesAround() const {

		return this->_numOfMinesAround;
	}



	void Cell::incrNumOfMinesAround() {

		++(this->_numOfMinesAround);
		assert(this->_numOfMinesAround >= 0 && this->_numOfMinesAround < 9);
	}



	void Cell::makeVisible() {

		this->_isVisible = true;
	}



	void Cell::putMine() {

		this->_hasMine = true;
	}



	void Cell::markCell() {

		assert(this->_isMarked != true);
		this->_isMarked = true;
	}



	void Cell::unmarkCell() {

		assert(this->_isMarked != false);
		this->_isMarked = false;
	}

}