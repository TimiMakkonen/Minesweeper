#ifndef MINESWEEPER_GAME_H
#define MINESWEEPER_GAME_H

#include <iostream>    // std::istream, std::ostream
#include <memory>      // std::unique_ptr
#include <type_traits> // std::enable_if
#include <vector>      // std::vector

#include <minesweeper/i_random.h>
#include <minesweeper/type_traits.h>
#include <minesweeper/visual_minesweeper_cell.h>

namespace minesweeper {

// declaration of 'Cell' class, properly included in 'game.cpp' file
class Cell;

class Game {
  private:
    // ---------------
    // fields:
    // ---------------

    int gridHeight = 0;
    int gridWidth = 0;
    int numOfMines = 0;
    int numOfMarkedMines = 0;
    int numOfWronglyMarkedCells = 0;
    int numOfVisibleCells = 0;
    bool _checkedMine = false;
    bool minesHaveBeenSet = false;

    std::vector<std::vector<std::unique_ptr<Cell>>> cells;

    // field used to randomise vector of ints to choose locations of mines
    IRandom* random = nullptr;

    // static field used to randomise vector of ints to choose locations of mines
    // only used if IRandom not specifically set for an instance
    // set with setDefaultRandom(IRandom* random)
    static IRandom* defaultRandom;

    static const int MAX_NUMBER_OF_CELLS_AROUND_MINE = 8;

  public:
    // ---------------
    // public methods:
    // ---------------

    Game();

    Game(int gridSize, int numOfMines, IRandom* random = nullptr);

    Game(int gridHeight, int gridWidth, int numOfMines, IRandom* random = nullptr);

    // default destructor
    ~Game();

    // optional now, invoked by: 'checkInputCoordinates' (left public for legacy support)
    void createMinesAndNums(const int initChosenX, const int initChosenY);

    void reset(bool keepCreatedMines = false);

    void newGame(int gridSize, int numOfMines);

    void newGame(int gridHeight, int gridWidth, int numOfMines);

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

    int getGridHeight() const;

    int getGridWidth() const;

    int getNumOfMines() const;

    // save game:
    std::ostream& serialise(std::ostream& outStream) const;

    // load game:
    std::istream& deserialise(std::istream& inStream);

    // get visual information of each cell
    template <class Container,
              typename std::enable_if<is_expandable_1D_sequence_container<Container>::value, int>::type = 0>
    Container visualise() const;
    template <class Container,
              typename std::enable_if<is_expandable_2D_sequence_container<Container>::value, int>::type = 0>
    Container visualise() const;

    template <class T = VisualMinesweeperCell>
    T visualiseCell(const int X, const int Y) const;

    // ----------------------
    // public static methods:
    // ----------------------

    static void setDefaultRandom(IRandom* random);

    static int maxNumOfMines(int gridHeight, int gridWidth);

    static int minNumOfMines();

    static int minNumOfMines(int gridHeight, int gridWidth);

  private:
    // ---------------
    // private methods:
    // ---------------

    std::vector<std::vector<std::unique_ptr<Cell>>> initCells() const;

    void resizeCells(int gridH, int gridW);

    void chooseRandomMineCells(std::vector<int>& mineSpots, const int initChosenX, const int initChosenY) const;

    void randomizeMineVector(std::vector<int>& mineSpots) const;

    void createMine(const int X, const int Y);

    void incrNumsAroundMine(const int X, const int Y);

    void checkAroundCoordinate(const int X, const int Y);

    bool allMinesMarked() const;

    bool noNonMinesMarked() const;

    bool allNonMinesVisible() const;

    bool checkedMine() const;

    void makeCellVisible(const int X, const int Y);

    void markCell(const int X, const int Y);

    void unmarkCell(const int X, const int Y);

    VisualMinesweeperCell visualiseCell_(const int X, const int Y) const;

    // -----------------------
    // private static methods:
    // -----------------------

    static int verifyGridDimension(int gridDimension);

    static int verifyNumOfMines(int numOfMines, int gridHeight, int gridWidth);

    static int verifyNumOfMarkedMines(int numOfMarkedMines, int numOfMines);

    static int verifyNumOfWronglyMarkedCells(int numOfWronglyMarkedCells, int gridHeight, int gridWidth,
                                             int numOfMines);

    static int verifyNumOfVisibleCells(int numOfVisibleCells, int gridHeight, int gridWidth);
};

} // namespace minesweeper

#include "game.tpp"

#endif // MINESWEEPER_GAME_H