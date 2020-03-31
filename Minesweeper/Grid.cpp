#include <stdexcept>
#include <vector>
#include <algorithm>
#include <memory>
#include <random>
#include <numeric>
#include <assert.h>

#include "Grid.h"

namespace Minesweeper {

	Grid::Grid(int gridSize, int numOfMines) : gridHeight(gridSize), gridWidth(gridSize), numOfMines(numOfMines), cells(this->initCells()) {}

	Grid::Grid(int gridHeight, int gridWidth, int numOfMines) : gridHeight(gridHeight), gridWidth(gridWidth), numOfMines(numOfMines), cells(this->initCells()) {}

	std::vector< std::vector< std::unique_ptr<typename Cell> > > Grid::initCells() {

		std::vector< std::vector< std::unique_ptr<Cell> > > initTempCells;
		initTempCells.reserve(this->gridHeight);
		for (int i = 0; i < this->gridHeight; ++i) {
			std::vector< std::unique_ptr<Cell> > tempVecOfCells;
			tempVecOfCells.reserve(this->gridWidth);
			for (int j = 0; j < this->gridWidth; ++j) {
				tempVecOfCells.emplace_back(std::make_unique<Cell>());
			}
			initTempCells.emplace_back(std::move(tempVecOfCells));
		}
		return initTempCells;
	}



	void Grid::createMinesAndNums(const int initChosenX, const int initChosenY) {

		std::vector<int> mineSpots(this->gridWidth * this->gridHeight);
		this->chooseRandomMineCells(mineSpots, initChosenX, initChosenY);

		int X, Y;
		for (int i = 0; i < this->numOfMines; ++i) {

			X = mineSpots[i] % this->gridWidth;
			Y = mineSpots[i] / this->gridWidth;

			this->createMine(X, Y);

			this->incrNumsAroundMine(X, Y);
		}
	}



	void Grid::chooseRandomMineCells(std::vector<int>& mineSpots, const int initChosenX, const int initChosenY) const {

		// to populate mineSpots vector with values: 0, 1, 2, ..., gridWidth*GridHeight - 1
		std::iota(mineSpots.begin(), mineSpots.end(), 0);

		// to shuffle this vector
		std::shuffle(mineSpots.begin(), mineSpots.end(), myRandomSeed);

		// to remove bad gridspots (those on and around chosen initial spot)
		mineSpots.erase(std::remove(mineSpots.begin(), mineSpots.end(),
			(initChosenY * this->gridWidth + initChosenX)), mineSpots.end());
		if (initChosenX > 0) {
			mineSpots.erase(std::remove(mineSpots.begin(), mineSpots.end(),
				(initChosenY * this->gridWidth + initChosenX - 1)), mineSpots.end());	//left
		}
		if (initChosenX < this->gridWidth - 1) {
			mineSpots.erase(std::remove(mineSpots.begin(), mineSpots.end(),
				(initChosenY * this->gridWidth + initChosenX + 1)), mineSpots.end());	// right
		}
		if (initChosenY > 0) {
			mineSpots.erase(std::remove(mineSpots.begin(), mineSpots.end(),
				((initChosenY - 1) * this->gridWidth + initChosenX)), mineSpots.end());	// top
		}
		if (initChosenY < this->gridHeight - 1) {
			mineSpots.erase(std::remove(mineSpots.begin(), mineSpots.end(),
				((initChosenY + 1) * this->gridWidth + initChosenX)), mineSpots.end());	// bottom
		}
		if (initChosenY > 0 && initChosenX > 0) {
			mineSpots.erase(std::remove(mineSpots.begin(), mineSpots.end(),
				((initChosenY - 1) * this->gridWidth + initChosenX - 1)), mineSpots.end());	// top left
		}
		if (initChosenY > 0 && initChosenX < this->gridWidth - 1) {
			mineSpots.erase(std::remove(mineSpots.begin(), mineSpots.end(),
				((initChosenY - 1) * this->gridWidth + initChosenX + 1)), mineSpots.end());	// top right
		}
		if (initChosenY < this->gridHeight - 1 && initChosenX > 0) {
			mineSpots.erase(std::remove(mineSpots.begin(), mineSpots.end(),
				((initChosenY + 1) * this->gridWidth + initChosenX - 1)), mineSpots.end());	// bottom left
		}
		if (initChosenY < this->gridHeight - 1 && initChosenX < this->gridWidth - 1) {
			mineSpots.erase(std::remove(mineSpots.begin(), mineSpots.end(),
				((initChosenY + 1) * this->gridWidth + initChosenX + 1)), mineSpots.end());	// bottom right
		}
	}

	

	void Grid::createMine(const int X, const int Y) {

		assert(X >= 0 && Y >= 0 && X < this->gridWidth && Y < this->gridHeight);
		this->cells[Y][X]->putMine();
	}



	void Grid::incrNumsAroundMine(const int X, const int Y) {

		assert(X >= 0 && Y >= 0 && X < this->gridWidth && Y < this->gridHeight);
		if (X > 0) {
			this->cells[Y][X - 1]->incrNumOfMinesAround();	//left
		}
		if (X < this->gridWidth - 1) {
			this->cells[Y][X + 1]->incrNumOfMinesAround();	// right
		}
		if (Y > 0) {
			this->cells[Y - 1][X]->incrNumOfMinesAround();	// top
		}
		if (Y < this->gridHeight - 1) {
			this->cells[Y + 1][X]->incrNumOfMinesAround();	// bottom
		}
		if (Y > 0 && X > 0) {
			this->cells[Y - 1][X - 1]->incrNumOfMinesAround();	// top left
		}
		if (Y > 0 && X < this->gridWidth - 1) {
			this->cells[Y - 1][X + 1]->incrNumOfMinesAround();	// top right
		}
		if (Y < this->gridHeight - 1 && X > 0) {
			this->cells[Y + 1][X - 1]->incrNumOfMinesAround();	// bottom left
		}
		if (Y < this->gridHeight - 1 && X < this->gridWidth - 1) {
			this->cells[Y + 1][X + 1]->incrNumOfMinesAround();	// bottom right
		}
	}


	// to mark (or unmark) given coordinates, and keeping track of marked and wrongly marked mines
	void Grid::markInputCoordinates(const int X, const int Y) {

		if (X < 0 || Y < 0 || X >= this->gridWidth || Y >= this->gridHeight) {
			throw std::out_of_range("Grid::markInputCoordinates(const int X, const int Y): Trying to mark cell outside grid.");
		}
		else if (this->cells[Y][X]->isMarked()) {
			this->cells[Y][X]->unmarkCell();

			if (this->cells[Y][X]->hasMine()) {
				--(this->numOfMarkedMines);
			}
			else {
				--(this->numOfWronglyMarkedMines);
			}
		}
		else {
			this->cells[Y][X]->markCell();

			if (this->cells[Y][X]->hasMine()) {
				++(this->numOfMarkedMines);
			}
			else {
				++(this->numOfWronglyMarkedMines);
			}
		}
	}



	bool Grid::allMinesMarked() const {

		if (this->numOfMines == this->numOfMarkedMines && this->numOfWronglyMarkedMines == 0) {
			return true;
		}
		else {
			return false;
		}
	}

	bool Grid::allNonMinesVisible() const {

		if (this->numOfVisibleCells + this->numOfMines == this->gridWidth * this->gridHeight && !this->_checkedMine) {
			return true;
		}
		else {
			return false;
		}
	}

	bool Grid::playerHasWon() const {

		if (this->allNonMinesVisible() || this->allMinesMarked()) {
			return true;
		}
		else {
			return false;
		}
	}

	bool Grid::playerHasLost() const {

		if (this->checkedMine()) {
			return true;
		}
		else {
			return false;
		}
	}


	bool Grid::isCellVisible(const int X, const int Y) const {

		return this->cells[Y][X]->isVisible();
	}

	bool Grid::doesCellHaveMine(const int X, const int Y) const {

		return this->cells[Y][X]->hasMine();
	}

	bool Grid::isCellMarked(const int X, const int Y) const {

		return this->cells[Y][X]->isMarked();
	}

	int Grid::numOfMinesAroundCell(const int X, const int Y) const {

		return this->cells[Y][X]->numOfMinesAround();
	}

	bool Grid::checkedMine() const {
		return this->_checkedMine;
	}



	// to check user given coordinates, and make it visible
	void Grid::checkInputCoordinates(const int X, const int Y) {

		if (X < 0 || Y < 0 || X >= this->gridWidth || Y >= this->gridHeight) {
			throw std::out_of_range("Grid::checkInputCoordinates(const int X, const int Y): Trying to check cell outside grid.");
		}
		else if (!(this->cells[Y][X]->isVisible()) && !(this->cells[Y][X]->isMarked())) {
			this->cells[Y][X]->makeVisible();
			++(this->numOfVisibleCells);

			if (this->cells[Y][X]->hasMine()) {
				this->_checkedMine = true;
			}
			else if (this->cells[Y][X]->numOfMinesAround() == 0) {
				this->checkAroundCoordinate(X, Y);
			}
		}
	}


	
	void Grid::checkAroundCoordinate(const int X, const int Y) {

		assert(X >= 0 && Y >= 0 && X < this->gridWidth && Y < this->gridHeight);
		if (X > 0) {
			this->checkInputCoordinates(X - 1, Y);		// left
		}
		if (X < this->gridWidth - 1) {
			this->checkInputCoordinates(X + 1, Y);		// right
		}
		if (Y > 0) {
			this->checkInputCoordinates(X, Y - 1);		// top
		}
		if (Y < this->gridHeight - 1) {
			this->checkInputCoordinates(X, Y + 1);		// bottom
		}
		if (X > 0 && Y > 0) {
			this->checkInputCoordinates(X - 1, Y - 1);	// top left
		}
		if (Y > 0 && X < this->gridWidth - 1) {
			this->checkInputCoordinates(X + 1, Y - 1);	// top right
		}
		if (Y < this->gridHeight - 1 && X > 0) {
			this->checkInputCoordinates(X - 1, Y + 1);	// bottom left
		}
		if (X < this->gridWidth - 1 && Y < this->gridHeight - 1) {
			this->checkInputCoordinates(X + 1, Y + 1);	// bottom right
		}
	}

}