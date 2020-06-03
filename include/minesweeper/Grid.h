#ifndef MINESWEEPER_GRID_H
#define MINESWEEPER_GRID_H

#include <vector>
#include <memory>
#include <random>

#include <minesweeper/IRandom.h>

namespace Minesweeper {

	// declaration of 'Cell' class, properly included in 'Grid.cpp' file
	class Cell;

	class Grid {
	private:

		// ---------------
		// fields:
		// ---------------

		int gridHeight;
		int gridWidth;
		int numOfMines;
		int numOfMarkedMines = 0;
		int numOfWronglyMarkedMines = 0;
		int numOfVisibleCells = 0;
		bool _checkedMine = false;

		std::vector< std::vector< std::unique_ptr<Cell> > > cells;

		// field used to randomise vector of ints to choose locations of mines
		IRandom* random;

		// ---------------
		// private methods:
		// ---------------

		int verifyNumOfMines(int numOfMines);

		std::vector< std::vector< std::unique_ptr<Cell> > > initCells();

		void chooseRandomMineCells(std::vector<int>& mineSpots, const int initChosenX, const int initChosenY) const;

		void randomizeMineVector(std::vector<int>& mineSpots) const;

		void createMine(const int X, const int Y);

		void incrNumsAroundMine(const int X, const int Y);

		void checkAroundCoordinate(const int X, const int Y);

		bool allMinesMarked() const;

		bool allNonMinesVisible() const;

		bool checkedMine() const;

		// static field used to randomise vector of ints to choose locations of mines
		// only used if IRandom not specifically set for an instance
		// set with setDefaultRandom(IRandom* random)
		static IRandom* defaultRandom;

	public:

		// ---------------
		// public methods:
		// ---------------

		Grid(int gridSize, int numOfMines, IRandom* random = nullptr);

		Grid(int gridHeight, int gridWidth, int numOfMines, IRandom* random = nullptr);

		// default
		~Grid();

		void createMinesAndNums(const int initChosenX, const int initChosenY);

		// to check user given coordinates, and make it visible
		void checkInputCoordinates(const int X, const int Y);

		// to mark (or unmark) given coordinates, and keeping track of marked and wrongly marked mines
		void markInputCoordinates(const int X, const int Y);

		bool playerHasWon() const;

		bool playerHasLost() const;

		bool isCellVisible(const int X, const int Y) const;

		bool doesCellHaveMine(const int X, const int Y) const;

		bool isCellMarked(const int X, const int Y) const;

		int numOfMinesAroundCell(const int X, const int Y) const;

		// ---------------
		// static methods:
		// ---------------

		static void setDefaultRandom(IRandom* random);

		static int maxNumOfMines(int gridHeight, int gridWidth);

		static int minNumOfMines();

		static int minNumOfMines(int gridHeight, int gridWidth);

	};

}

#endif // MINESWEEPER_GRID_H