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
    // +---------+
    // | fields: |
    // +---------+

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
    // +-----------------+
    // | public methods: |
    // +-----------------+

    // constructors:
    Game();
    Game(int gridSize, int numOfMines, IRandom* random = nullptr);
    Game(int gridHeight, int gridWidth, int numOfMines, IRandom* random = nullptr);
    Game(int gridSize, double proportionOfMines, IRandom* random = nullptr);
    Game(int gridHeight, int gridWidth, double proportionOfMines, IRandom* random = nullptr);

    // default destructor:
    ~Game();

    // reset and new game methods:
    void reset(bool keepCreatedMines = false);
    void newGame(int gridSize, int numOfMines);
    void newGame(int gridHeight, int gridWidth, int numOfMines);
    void newGame(int gridSize, double proportionOfMines);
    void newGame(int gridHeight, int gridWidth, double proportionOfMines);

    // optional (and not recommended) now, invoked by: 'checkInputCoordinates' (left available for legacy support)
    void createMinesAndNums(const int initChosenX, const int initChosenY);

    // to check user given coordinates, and make it visible
    void checkInputCoordinates(const int X, const int Y);

    // to mark (or unmark) given coordinates, and keeping track of marked and wrongly marked mines
    void markInputCoordinates(const int X, const int Y);

    // game progress information:
    bool playerHasWon() const;
    bool playerHasLost() const;

    // cell information:
    bool isCellVisible(const int X, const int Y) const;
    bool doesCellHaveMine(const int X, const int Y) const;
    bool isCellMarked(const int X, const int Y) const;
    int numOfMinesAroundCell(const int X, const int Y) const;

    // getters:
    int getGridHeight() const;
    int getGridWidth() const;
    int getNumOfMines() const;

    // save game:
    std::ostream& serialise(std::ostream& outStream) const;

    // load game:
    std::istream& deserialise(std::istream& inStream);

    // get visual information of cell:
    template <class T = VisualMinesweeperCell>
    T visualiseCell(const int X, const int Y) const;

    // get visual information of each cell:
    template <class Container,
              typename std::enable_if<is_expandable_1D_sequence_container<Container>::value, int>::type = 0>
    Container visualise() const;
    template <class Container,
              typename std::enable_if<is_expandable_2D_sequence_container<Container>::value, int>::type = 0>
    Container visualise() const;

    // +------------------------+
    // | public static methods: |
    // +------------------------+

    // static default random setter:
    static void setDefaultRandom(IRandom* random);

    // static maximum number/proportion of mines checking methods:
    static int maxNumOfMines(const int gridHeight, const int gridWidth);
    static double maxProportionOfMines(const int gridHeight, const int gridWidth);

    // static minimum number/proportion of mines checking methods:
    static int minNumOfMines();
    static int minNumOfMines(const int gridHeight, const int gridWidth);
    static double minProportionOfMines();
    static double minProportionOfMines(const int gridHeight, const int gridWidth);

  private:
    // +------------------+
    // | private methods: |
    // +------------------+

    // private resizeCells method:
    void resizeCells(int gridH, int gridW);

    // private mines creation methods:
    void createMinesAndNums_(const int initChosenX, const int initChosenY);
    void chooseRandomMineCells(std::vector<int>& mineSpots, const int initChosenX, const int initChosenY) const;
    void randomizeMineVector(std::vector<int>& mineSpots) const;
    void createMine(const int X, const int Y);
    void incrNumsAroundMine(const int X, const int Y);

    // private reset method:
    void reset_(bool keepCreatedMines = false);

    // private check cell methods:
    void checkInputCoordinates_(const int X, const int Y);
    void checkAroundCoordinate(const int X, const int Y);
    void makeCellVisible(const int X, const int Y);

    // private mark cell methods:
    void markCell(const int X, const int Y);
    void unmarkCell(const int X, const int Y);

    // private win conditions:
    bool allNonMinesVisible() const;
    bool allMinesMarked() const;
    bool noNonMinesMarked() const;

    bool playerHasLost_() const;

    // private loss conditions:
    bool checkedMine() const;

    // private cell information methods:
    bool isCellVisible_(const int X, const int Y) const;
    bool doesCellHaveMine_(const int X, const int Y) const;
    bool isCellMarked_(const int X, const int Y) const;
    int numOfMinesAroundCell_(const int X, const int Y) const;

    // private visual cell information:
    VisualMinesweeperCell visualiseCell_(const int X, const int Y) const;

    // +-------------------------+
    // | private static methods: |
    // +-------------------------+

    // private static cell initialisation method:
    static std::vector<std::vector<std::unique_ptr<Cell>>> initCells(const int gridH, const int gridW);

    // private static verification methods:
    static int verifyGridDimension(int gridDimension);
    static int verifyNumOfMines(int numOfMines, int gridHeight, int gridWidth);
    static double verifyProportionOfMines(double proportionOfMines, int gridHeight, int gridWidth);
    static int verifyNumOfVisibleCells(int numOfVisibleCells, int gridHeight, int gridWidth);
    static int verifyNumOfMarkedMines(int numOfMarkedMines, int numOfMines);
    static int verifyNumOfWronglyMarkedCells(int numOfWronglyMarkedCells, int gridHeight, int gridWidth,
                                             int numOfMines);
};

} // namespace minesweeper

#include "game.tpp"

#endif // MINESWEEPER_GAME_H