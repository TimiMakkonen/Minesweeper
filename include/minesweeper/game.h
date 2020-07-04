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

    int _gridHeight = 0;
    int _gridWidth = 0;
    int _numOfMines = 0;
    int _numOfMarkedMines = 0;
    int _numOfWronglyMarkedCells = 0;
    int _numOfVisibleCells = 0;
    bool _checkedMine = false;
    bool _minesHaveBeenSet = false;

    std::vector<std::vector<std::unique_ptr<Cell>>> _cells;

    // field used to randomise vector of ints to choose locations of mines
    IRandom* _random = nullptr;

    // static field used to randomise vector of ints to choose locations of mines
    // only used if IRandom not specifically set for an instance
    // set with setDefaultRandom(IRandom* random)
    static IRandom* defaultRandom;

  public:
    // +-----------------+
    // | public methods: |
    // +-----------------+

    // constructors:
    Game();
    Game(const Game& other);
    Game(Game&& other) noexcept;
    Game(int gridSize, int numOfMines, IRandom* random = nullptr);
    Game(int gridHeight, int gridWidth, int numOfMines, IRandom* random = nullptr);
    Game(int gridSize, double proportionOfMines, IRandom* random = nullptr);
    Game(int gridHeight, int gridWidth, double proportionOfMines, IRandom* random = nullptr);

    // default destructor:
    ~Game();

    // assignment operators:
    Game& operator=(Game other);
    Game& operator=(Game&& other) noexcept;

    // swap:
    friend void swap(Game& first, Game& second);

    // reset and new game methods:
    void reset(bool keepCreatedMines = false);
    void newGame(int gridSize, int numOfMines);
    void newGame(int gridHeight, int gridWidth, int numOfMines);
    void newGame(int gridSize, double proportionOfMines);
    void newGame(int gridHeight, int gridWidth, double proportionOfMines);

    // optional (and not recommended) now, invoked by: 'checkInputCoordinates' (left available for legacy support)
    void createMinesAndNums(int initChosenX, int initChosenY);

    // to check user given coordinates, and make it visible
    void checkInputCoordinates(int x, int y);

    // to mark (or unmark) given coordinates, and keeping track of marked and wrongly marked mines
    void markInputCoordinates(int x, int y);

    // game progress information:
    bool playerHasWon() const;
    bool playerHasLost() const;

    // cell information:
    bool isCellVisible(int x, int y) const;
    bool doesCellHaveMine(int x, int y) const;
    bool isCellMarked(int x, int y) const;
    int numOfMinesAroundCell(int x, int y) const;

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
    T visualiseCell(int x, int y) const;

    // get visual information of each cell:
    template <class Container,
              typename std::enable_if<is_expandable_1d_sequence_container<Container>::value, int>::type = 0>
    Container visualise() const;
    template <class Container,
              typename std::enable_if<is_expandable_2d_sequence_container<Container>::value, int>::type = 0>
    Container visualise() const;

    // +------------------------+
    // | public static methods: |
    // +------------------------+

    // static default random setter:
    static void setDefaultRandom(IRandom* random);

    // static maximum number/proportion of mines checking methods:
    static int maxNumOfMines(int gridHeight, int gridWidth);
    static double maxProportionOfMines(int gridHeight, int gridWidth);

    // static minimum number/proportion of mines checking methods:
    static int minNumOfMines();
    static int minNumOfMines(int gridHeight, int gridWidth);
    static double minProportionOfMines();
    static double minProportionOfMines(int gridHeight, int gridWidth);

  private:
    // +----------------------+
    // | static const fields: |
    // +----------------------+

    static const int MAX_NUMBER_OF_CELLS_AROUND_MINE;

    // serialisation magic/version keys:
    static const char* MAGIC_KEY;
    static const char* VERSION_KEY;
    // basic serialisation structure keys:
    static const char* CURRENT_GAME_KEY;
    static const char* ROW_NUMBER_KEY;
    static const char* COLUMN_NUMBER_KEY;
    static const char* CELLS_KEY;
    static const char* ROW_CELLS_KEY;
    static const char* CELL_KEY;
    // Game private member keys:
    static const char* GRID_HEIGHT_KEY;
    static const char* GRID_WIDTH_KEY;
    static const char* NUM_OF_MINES_KEY;
    static const char* NUM_OF_MARKED_MINES_KEY;
    static const char* NUM_OF_WRONGLY_MARKED_CELLS_KEY;
    static const char* NUM_OF_VISIBLE_CELLS_KEY;
    static const char* CHECKED_MINE_KEY;
    static const char* MINES_HAVE_BEEN_SET_KEY;

    // +------------------+
    // | private methods: |
    // +------------------+

    // private resizeCells method:
    void resizeCells_(int gridH, int gridW);

    // private mines creation methods:
    void createMinesAndNums_(int initChosenX, int initChosenY);
    void chooseRandomMineCells_(std::vector<int>& mineSpots, int initChosenX, int initChosenY) const;
    void randomizeMineVector_(std::vector<int>& mineSpots) const;
    void createMine_(int x, int y);
    void incrNumsAroundMine_(int x, int y);

    // private reset method:
    void reset_(bool keepCreatedMines = false);

    // private check cell methods:
    void checkInputCoordinates_(int x, int y);
    void checkAroundCoordinate_(int x, int y);
    void makeCellVisible_(int x, int y);

    // private mark cell methods:
    void markCell_(int x, int y);
    void unmarkCell_(int x, int y);

    // private win conditions:
    bool allNonMinesVisible_() const;
    bool allMinesMarked_() const;
    bool noNonMinesMarked_() const;

    bool playerHasLost_() const;

    // private loss conditions:
    bool checkedMine_() const;

    // private cell information methods:
    bool isCellVisible_(int x, int y) const;
    bool doesCellHaveMine_(int x, int y) const;
    bool isCellMarked_(int x, int y) const;
    int numOfMinesAroundCell_(int x, int y) const;

    // private visual cell information:
    VisualMinesweeperCell visualiseCell_(int x, int y) const;

    // +-------------------------+
    // | private static methods: |
    // +-------------------------+

    // private static cell initialisation methods:
    static std::vector<std::vector<std::unique_ptr<Cell>>> initCells_(int gridHeight, int gridWidth);
    static std::vector<std::vector<std::unique_ptr<Cell>>> initCells_(const Game& other);

    // private static verification methods:
    static int verifyGridDimension_(int gridDimension);
    static int verifyNumOfMines_(int numOfMines, int gridHeight, int gridWidth);
    static double verifyProportionOfMines_(double proportionOfMines, int gridHeight, int gridWidth);
    static int verifyNumOfVisibleCells_(int numOfVisibleCells, int gridHeight, int gridWidth);
    static int verifyNumOfMarkedMines_(int numOfMarkedMines, int numOfMines);
    static int verifyNumOfWronglyMarkedCells_(int numOfWronglyMarkedCells, int gridHeight, int gridWidth,
                                              int numOfMines);
};

} // namespace minesweeper

#include "game.tpp"

#endif // MINESWEEPER_GAME_H