#include <algorithm> // std::max
#include <cassert>   // assert
#include <iomanip>   // std::setw
#include <memory>    // std::unique_ptr, std::make_unique (C++14)
#include <numeric>   // std::iota
#include <sstream>   // std::ostringstream
#include <stdexcept> // std::out_of_range, std::invalid_argument
#include <string>    // std::string
#include <utility>   // std::pair, std::make_pair
#include <vector>    // std::vector

#include <json.hpp> // nlohmann::json

#include <minesweeper/cell.h>
#include <minesweeper/game.h>
#include <minesweeper/visual_minesweeper_cell.h>

namespace minesweeper {

// +----------------------------------+
// | initialisation of static fields: |
// +----------------------------------+

IRandom* Game::defaultRandom = nullptr;

const int Game::MAX_NUMBER_OF_CELLS_AROUND_MINE = 8;

// serialisation magic/version keys:
const char* Game::MAGIC_KEY = "magic";
const char* Game::VERSION_KEY = "version";
// basic serialisation structure keys:
const char* Game::CURRENT_GAME_KEY = "currentGame";
const char* Game::ROW_NUMBER_KEY = "rowNumber";
const char* Game::COLUMN_NUMBER_KEY = "columnNumber";
const char* Game::CELLS_KEY = "cells";
const char* Game::ROW_CELLS_KEY = "rowCells";
const char* Game::CELL_KEY = "cell";
// Game private member keys:
const char* Game::GRID_HEIGHT_KEY = "gridHeight";
const char* Game::GRID_WIDTH_KEY = "gridWidth";
const char* Game::NUM_OF_MINES_KEY = "numOfMines";
const char* Game::NUM_OF_MARKED_MINES_KEY = "numOfMarkedMines";
const char* Game::NUM_OF_WRONGLY_MARKED_CELLS_KEY = "numOfWronglyMarkedCells";
const char* Game::NUM_OF_VISIBLE_CELLS_KEY = "numOfVisibleCells";
const char* Game::CHECKED_MINE_KEY = "_checkedMine";
const char* Game::MINES_HAVE_BEEN_SET_KEY = "minesHaveBeenSet";

// +----------+
// | methods: |
// +----------+

Game::Game() = default;

Game::Game(const Game& other)
    : _gridHeight(other._gridHeight), _gridWidth(other._gridWidth), _numOfMines(other._numOfMines),
      _numOfMarkedMines(other._numOfMarkedMines), _numOfWronglyMarkedCells(other._numOfWronglyMarkedCells),
      _numOfVisibleCells(other._numOfVisibleCells), _checkedMine(other._checkedMine),
      _minesHaveBeenSet(other._minesHaveBeenSet), _cells{initCells_(other)}, _random(other._random) {}

Game::Game(Game&& other) noexcept : Game() { swap(*this, other); }

Game::Game(int gridSize, int numOfMines, IRandom* random) : Game(gridSize, gridSize, numOfMines, random) {}

Game::Game(int gridHeight, int gridWidth, int numOfMines, IRandom* random)
    : _gridHeight{verifyGridDimension_(gridHeight)},                     // throws
      _gridWidth{verifyGridDimension_(gridWidth)},                       // throws
      _numOfMines{verifyNumOfMines_(numOfMines, gridHeight, gridWidth)}, // throws
      _cells{initCells_(this->_gridHeight, this->_gridWidth)}, _random{random} {}

Game::Game(int gridSize, double proportionOfMines, IRandom* random)
    : Game(gridSize, gridSize, proportionOfMines, random) {}

Game::Game(int gridHeight, int gridWidth, double proportionOfMines, IRandom* random)
    : _gridHeight{verifyGridDimension_(gridHeight)}, // throws
      _gridWidth{verifyGridDimension_(gridWidth)},
      _numOfMines{verifyNumOfMines_(
          static_cast<int>(verifyProportionOfMines_(proportionOfMines, gridHeight, gridWidth) * gridHeight * gridWidth),
          gridHeight, gridWidth)}, // throws
      _cells{initCells_(this->_gridHeight, this->_gridWidth)}, _random{random} {}

// required by to solve "error C2027: use of undefined type"
// in short, std::unique_ptr requires destructor to be defined here
// Check more info on this:
// https://stackoverflow.com/questions/40383395/use-of-undefined-type-with-unique-ptr-to-forward-declared-class-and-defaulted
// https://stackoverflow.com/questions/6012157/is-stdunique-ptrt-required-to-know-the-full-definition-of-t
// https://stackoverflow.com/questions/13414652/forward-declaration-with-unique-ptr
Game::~Game() = default;

Game& Game::operator=(Game other) {

    swap(*this, other);

    return *this;
}

// friend swap function
void swap(Game& first, Game& second) {

    using std::swap;

    swap(first._gridHeight, second._gridHeight);
    swap(first._gridWidth, second._gridWidth);
    swap(first._numOfMines, second._numOfMines);

    swap(first._numOfMarkedMines, second._numOfMarkedMines);
    swap(first._numOfWronglyMarkedCells, second._numOfWronglyMarkedCells);
    swap(first._numOfVisibleCells, second._numOfVisibleCells);

    swap(first._checkedMine, second._checkedMine);
    swap(first._minesHaveBeenSet, second._minesHaveBeenSet);

    swap(first._cells, second._cells);

    swap(first._random, second._random);
}

// static method
int Game::verifyGridDimension_(int gridDimension) {

    if (gridDimension < 0) {
        throw std::out_of_range(
            "Game::verifyGridDimension_(int gridDimension): Trying to create a grid with negative (" +
            std::to_string(gridDimension) + ") grid dimension.");
    }

    return gridDimension;
}

// static method
int Game::verifyNumOfMines_(int numOfMines, int gridHeight, int gridWidth) {

    if (numOfMines > maxNumOfMines(gridHeight, gridWidth)) {
        throw std::out_of_range("Game::verifyNumOfMines_(int numOfMines, int gridHeight, int gridWidth): "
                                "Trying to create a grid with too many (" +
                                std::to_string(numOfMines) + ") mines.");
    }

    if (numOfMines < minNumOfMines(gridHeight, gridWidth)) {
        throw std::out_of_range("Game::verifyNumOfMines_(int numOfMines, int gridHeight, int gridWidth): "
                                "Trying to create a grid with too few (" +
                                std::to_string(numOfMines) + ") mines.");
    }

    return numOfMines;
}

// static method
double Game::verifyProportionOfMines_(double proportionOfMines, int gridHeight, int gridWidth) {

    if (proportionOfMines > maxProportionOfMines(gridHeight, gridWidth)) {
        throw std::out_of_range(
            "Game::verifyProportionOfMines_(double proportionOfMines, int gridHeight, int gridWidth): "
            "Trying to create a grid with too big proportion (" +
            std::to_string(proportionOfMines) + ") of mines.");
    }

    if (proportionOfMines < minProportionOfMines(gridHeight, gridWidth)) {
        throw std::out_of_range(
            "Game::verifyProportionOfMines_(double proportionOfMines, int gridHeight, int gridWidth): "
            "Trying to create a grid with too small proportion (" +
            std::to_string(proportionOfMines) + ") of mines.");
    }

    return proportionOfMines;
}

// static method
int Game::verifyNumOfMarkedMines_(int numOfMarkedMines, int numOfMines) {

    if (numOfMarkedMines > numOfMines) {
        throw std::out_of_range("Game::verifyNumOfMarkedMines_(int numOfMarkedMines, int numOfMines): "
                                "Trying to create a grid with too many (" +
                                std::to_string(numOfMarkedMines) + ") marked mines.");
    }

    if (numOfMarkedMines < 0) {
        throw std::out_of_range("Game::verifyNumOfMarkedMines_(int numOfMarkedMines, int numOfMines): "
                                "Trying to create a grid with too few (" +
                                std::to_string(numOfMarkedMines) + ") marked mines.");
    }

    return numOfMarkedMines;
}

// static method
int Game::verifyNumOfWronglyMarkedCells_(int numOfWronglyMarkedCells, int gridHeight, int gridWidth, int numOfMines) {

    int numOfMinelessCells = gridHeight * gridWidth - numOfMines;

    if (numOfWronglyMarkedCells > numOfMinelessCells) {
        throw std::out_of_range("Game::verifyNumOfWronglyMarkedCells_("
                                "int numOfWronglyMarkedCells, int gridHeight, int gridWidth, int numOfMines): "
                                "Trying to create a grid with too many (" +
                                std::to_string(numOfWronglyMarkedCells) + ") wrongly marked cells.");
    }

    if (numOfWronglyMarkedCells < 0) {
        throw std::out_of_range("Game::verifyNumOfWronglyMarkedCells_("
                                "int numOfWronglyMarkedCells, int gridHeight, int gridWidth, int numOfMines): "
                                "Trying to create a grid with too few (" +
                                std::to_string(numOfWronglyMarkedCells) + ") wrongly marked cells.");
    }

    return numOfWronglyMarkedCells;
}

// static method
int Game::verifyNumOfVisibleCells_(int numOfVisibleCells, int gridHeight, int gridWidth) {

    if (numOfVisibleCells > gridHeight * gridWidth) {
        throw std::out_of_range("Game::verifyNumOfVisibleCells_(int numOfVisibleCells, int gridHeight, int gridWidth): "
                                "Trying to create a grid with too many (" +
                                std::to_string(numOfVisibleCells) + ") visible cells.");
    }

    if (numOfVisibleCells < 0) {
        throw std::out_of_range("Game::verifyNumOfVisibleCells_(int numOfVisibleCells, int gridHeight, int gridWidth): "
                                "Trying to create a grid with too few (" +
                                std::to_string(numOfVisibleCells) + ") visible cells.");
    }

    return numOfVisibleCells;
}

// static method
// consider combining with Game::resizeCells()
std::vector<std::vector<std::unique_ptr<Cell>>> Game::initCells_(const int gridHeight, const int gridWidth) {

    assert(gridHeight >= 0 && gridWidth >= 0);

    std::vector<std::vector<std::unique_ptr<Cell>>> outputCellMatrix;
    outputCellMatrix.reserve(gridHeight);
    for (int y = 0; y < gridHeight; ++y) {
        std::vector<std::unique_ptr<Cell>> outputCellRow;
        outputCellRow.reserve(gridWidth);
        for (int x = 0; x < gridWidth; ++x) {
            outputCellRow.emplace_back(std::make_unique<Cell>());
        }
        outputCellMatrix.emplace_back(std::move(outputCellRow));
    }
    return outputCellMatrix;
}

// static method
// consider combining with Game::resizeCells()
std::vector<std::vector<std::unique_ptr<Cell>>> Game::initCells_(const Game& other) {

    auto gridHeight = other._gridHeight;
    auto gridWidth = other._gridWidth;

    std::vector<std::vector<std::unique_ptr<Cell>>> outputCellMatrix;
    outputCellMatrix.reserve(gridHeight);
    for (const auto& otherCellRow : other._cells) {
        std::vector<std::unique_ptr<Cell>> outputCellRow;
        outputCellRow.reserve(gridWidth);
        for (const auto& otherCellPtr : otherCellRow) {
            outputCellRow.emplace_back(std::make_unique<Cell>(*otherCellPtr));
        }
        outputCellMatrix.emplace_back(std::move(outputCellRow));
    }
    return outputCellMatrix;
}

// consider combining with Game::initCells_()
void Game::resizeCells_(const int gridHeight, const int gridWidth) {

    assert(gridHeight >= 0 && gridWidth >= 0);

    this->_cells.reserve(gridHeight);
    this->_cells.resize(gridHeight);
    for (auto& cellRow : _cells) {

        cellRow.reserve(gridWidth);
        if (cellRow.size() < gridWidth) {
            for (size_t i = cellRow.size(); i < gridWidth; ++i) {
                cellRow.emplace_back(std::make_unique<Cell>());
            }
        }
        cellRow.resize(gridWidth);
    }
}

void Game::createMinesAndNums(const int initChosenX, const int initChosenY) {

    if (initChosenX < 0 || initChosenY < 0 || initChosenX >= this->_gridWidth || initChosenY >= this->_gridHeight) {
        throw std::out_of_range("Game::createMinesAndNums(const int initChosenX, const int initChosenY): "
                                "Trying to use initial cell (initChosenX, initChosenX) outside grid.");
    }

    if (this->_minesHaveBeenSet) {
        throw std::invalid_argument("Game::createMinesAndNums(const int initChosenX, const int initChosenY): "
                                    "Trying to create mines and numbers for a grid that already has them created.");
    }

    this->createMinesAndNums_(initChosenX, initChosenY);
}

void Game::createMinesAndNums_(const int initChosenX, const int initChosenY) {

    assert(initChosenX >= 0 && initChosenY >= 0 && initChosenX < this->_gridWidth && initChosenY < this->_gridHeight);

    assert(!this->_minesHaveBeenSet);

    std::vector<int> mineSpots(this->_gridWidth * this->_gridHeight);
    this->chooseRandomMineCells_(mineSpots, initChosenX, initChosenY);

    int x = 0;
    int y = 0;
    for (int i = 0; i < this->_numOfMines; ++i) {

        x = mineSpots[i] % this->_gridWidth;
        y = mineSpots[i] / this->_gridWidth;

        this->createMine_(x, y);

        this->incrNumsAroundMine_(x, y);
    }

    this->_minesHaveBeenSet = true;
}

void Game::chooseRandomMineCells_(std::vector<int>& mineSpots, const int initChosenX, const int initChosenY) const {

    // to populate mineSpots vector with values: 0, 1, 2, ..., gridWidth*gridHeight - 1
    std::iota(mineSpots.begin(), mineSpots.end(), 0);

    // to shuffle this vector
    this->randomizeMineVector_(mineSpots);

    // to remove bad gridspots (those on and around chosen initial spot)
    mineSpots.erase(std::remove(mineSpots.begin(), mineSpots.end(), (initChosenY * this->_gridWidth + initChosenX)),
                    mineSpots.end());
    for (auto const& neighbourCell : this->neighbourCells_(initChosenX, initChosenY)) {
        mineSpots.erase(std::remove(mineSpots.begin(), mineSpots.end(),
                                    neighbourCell.second * this->_gridWidth + neighbourCell.first),
                        mineSpots.end());
    }
}

void Game::randomizeMineVector_(std::vector<int>& mineSpots) const {

    if (this->_random != nullptr) {
        this->_random->shuffleVector(mineSpots);
    } else {
        if (Game::defaultRandom != nullptr) {
            Game::defaultRandom->shuffleVector(mineSpots);
        } else {
            throw std::invalid_argument("Game::randomizeMineVector_(std::vector<int>& mineSpots): "
                                        "Neither field '_random' nor static field 'defaultRandom' is initialised.");
        }
    }
}

void Game::createMine_(const int x, const int y) {

    assert(x >= 0 && y >= 0 && x < this->_gridWidth && y < this->_gridHeight);
    this->_cells[y][x]->putMine();
}

void Game::incrNumsAroundMine_(const int x, const int y) {

    assert(x >= 0 && y >= 0 && x < this->_gridWidth && y < this->_gridHeight);

    for (auto const& neighbourCell : this->neighbourCells_(x, y)) {
        this->_cells[neighbourCell.second][neighbourCell.first]->incrNumOfMinesAround();
    }
}

void Game::reset(bool keepCreatedMines) { this->reset_(keepCreatedMines); }

void Game::reset_(bool keepCreatedMines) {

    // reset game fields
    this->_numOfMarkedMines = 0;
    this->_numOfWronglyMarkedCells = 0;
    this->_numOfVisibleCells = 0;
    this->_checkedMine = false;

    if (!keepCreatedMines) {
        this->_minesHaveBeenSet = false;
    }

    // reset cells
    for (auto& cellRow : this->_cells) {
        for (auto& cell : cellRow) {

            cell->reset(keepCreatedMines);
        }
    }
}

void Game::newGame(int gridSize, int numOfMines) { this->newGame(gridSize, gridSize, numOfMines); }

void Game::newGame(int gridHeight, int gridWidth, int numOfMines) {

    // parameter verification
    verifyGridDimension_(gridHeight);
    verifyGridDimension_(gridWidth);
    verifyNumOfMines_(numOfMines, gridHeight, gridWidth);

    this->reset_(false);
    this->resizeCells_(gridHeight, gridWidth);
    this->_gridHeight = gridHeight;
    this->_gridWidth = gridWidth;
    this->_numOfMines = numOfMines;
}

void Game::newGame(int gridSize, double proportionOfMines) { this->newGame(gridSize, gridSize, proportionOfMines); }

void Game::newGame(int gridHeight, int gridWidth, double proportionOfMines) {

    // parameter verification
    verifyGridDimension_(gridHeight);
    verifyGridDimension_(gridWidth);
    verifyProportionOfMines_(proportionOfMines, gridHeight, gridWidth);
    int newNumOfMines = static_cast<int>(proportionOfMines * gridHeight * gridWidth);
    verifyNumOfMines_(newNumOfMines, gridHeight, gridWidth);

    this->reset_(false);
    this->resizeCells_(gridHeight, gridWidth);
    this->_gridHeight = gridHeight;
    this->_gridWidth = gridWidth;
    this->_numOfMines = newNumOfMines;
}

// to mark (or unmark) given coordinates, and keeping track of marked and wrongly marked mines
void Game::markInputCoordinates(const int x, const int y) {

    if (x < 0 || y < 0 || x >= this->_gridWidth || y >= this->_gridHeight) {
        throw std::out_of_range(
            "Game::markInputCoordinates(const int x, const int y): Trying to mark cell outside grid.");
    }

    if (!this->_minesHaveBeenSet) {
        throw std::invalid_argument("Game::markInputCoordinates(const int x, const int y): "
                                    "Trying to mark a cell before mines have been initialised. \n\t"
                                    "(Initialise mines by calling: "
                                    "createMinesAndNums(const int initChosenX, const int initChosenY) or "
                                    "checkInputCoordinates(const int x, const int y).)");
    }

    // if cell is already visible, do nothing
    // else mark or unmark it and update 'marked counters'
    if (!this->isCellVisible_(x, y)) {
        if (this->isCellMarked_(x, y)) {
            this->unmarkCell_(x, y);

            if (this->doesCellHaveMine_(x, y)) {
                --(this->_numOfMarkedMines);
            } else {
                --(this->_numOfWronglyMarkedCells);
            }
        } else {
            if (!this->isCellVisible_(x, y)) {

                this->markCell_(x, y);

                if (this->doesCellHaveMine_(x, y)) {
                    ++(this->_numOfMarkedMines);
                } else {
                    ++(this->_numOfWronglyMarkedCells);
                }
            }
        }
    }
}

bool Game::allMinesMarked_() const { return this->_numOfMines == this->_numOfMarkedMines; }

bool Game::noNonMinesMarked_() const { return this->_numOfWronglyMarkedCells == 0; }

bool Game::allNonMinesVisible_() const {

    return this->_numOfVisibleCells + this->_numOfMines == this->_gridWidth * this->_gridHeight && !this->_checkedMine;
}

bool Game::playerHasWon() const {

    bool playerWon = false;

    if (!this->playerHasLost_()) {
        if (this->allNonMinesVisible_() || (this->allMinesMarked_() && this->noNonMinesMarked_())) {
            playerWon = true;
        }
    }
    return playerWon;
}

bool Game::playerHasLost() const { return this->playerHasLost_(); }

bool Game::playerHasLost_() const { return this->checkedMine_(); }

bool Game::checkedMine_() const { return this->_checkedMine; }

bool Game::isCellVisible(const int x, const int y) const {

    if (x < 0 || y < 0 || x >= this->_gridWidth || y >= this->_gridHeight) {
        throw std::out_of_range(
            "Game::isCellVisible(const int x, const int y): Trying to check if a cell outside the grid is visible.");
    }
    return this->isCellVisible_(x, y);
}

bool Game::isCellVisible_(const int x, const int y) const {

    assert(x >= 0 && y >= 0 && x < this->_gridWidth && y < this->_gridHeight);

    return this->_cells[y][x]->isVisible();
}

bool Game::doesCellHaveMine(const int x, const int y) const {

    if (x < 0 || y < 0 || x >= this->_gridWidth || y >= this->_gridHeight) {
        throw std::out_of_range(
            "Game::doesCellHaveMine(const int x, const int y): Trying to check if a cell outside the grid has a mine.");
    }
    return this->doesCellHaveMine_(x, y);
}

bool Game::doesCellHaveMine_(const int x, const int y) const {

    assert(x >= 0 && y >= 0 && x < this->_gridWidth && y < this->_gridHeight);

    return this->_cells[y][x]->hasMine();
}

bool Game::isCellMarked(const int x, const int y) const {

    if (x < 0 || y < 0 || x >= this->_gridWidth || y >= this->_gridHeight) {
        throw std::out_of_range(
            "Game::isCellMarked(const int x, const int y): Trying to check if a cell outside the grid is marked.");
    }
    return this->isCellMarked_(x, y);
}

bool Game::isCellMarked_(const int x, const int y) const {

    assert(x >= 0 && y >= 0 && x < this->_gridWidth && y < this->_gridHeight);

    return this->_cells[y][x]->isMarked();
}

int Game::numOfMinesAroundCell(const int x, const int y) const {

    if (x < 0 || y < 0 || x >= this->_gridWidth || y >= this->_gridHeight) {
        throw std::out_of_range("Game::numOfMinesAroundCell(const int x, const int y): "
                                "Trying to check number of mines around a cell outside the grid.");
    }
    return this->numOfMinesAroundCell_(x, y);
}

int Game::numOfMinesAroundCell_(const int x, const int y) const {

    assert(x >= 0 && y >= 0 && x < this->_gridWidth && y < this->_gridHeight);

    return this->_cells[y][x]->numOfMinesAround();
}

void Game::makeCellVisible_(const int x, const int y) {
    assert(x >= 0 && y >= 0 && x < this->_gridWidth && y < this->_gridHeight);
    this->_cells[y][x]->makeVisible();
}

void Game::markCell_(const int x, const int y) {
    assert(x >= 0 && y >= 0 && x < this->_gridWidth && y < this->_gridHeight);
    this->_cells[y][x]->markCell();
}

void Game::unmarkCell_(const int x, const int y) {
    assert(x >= 0 && y >= 0 && x < this->_gridWidth && y < this->_gridHeight);
    this->_cells[y][x]->unmarkCell();
}

VisualMinesweeperCell Game::visualiseCell_(const int x, const int y) const {
    assert(x >= 0 && y >= 0 && x < this->_gridWidth && y < this->_gridHeight);
    return this->_cells[y][x]->visualise();
}

VisualMinesweeperCell Game::visualiseCellSolution_(const int x, const int y) const {
    assert(x >= 0 && y >= 0 && x < this->_gridWidth && y < this->_gridHeight);
    return this->_cells[y][x]->visualiseSolution();
}

// to check user given coordinates, and make it visible
void Game::checkInputCoordinates(const int x, const int y) {

    if (x < 0 || y < 0 || x >= this->_gridWidth || y >= this->_gridHeight) {
        throw std::out_of_range(
            "Game::checkInputCoordinates(const int x, const int y): Trying to check cell outside grid.");
    }

    this->checkInputCoordinates_(x, y);
}

void Game::checkInputCoordinates_(const int x, const int y) {

    assert(x >= 0 && y >= 0 && x < this->_gridWidth && y < this->_gridHeight);

    // could maybe be moved to public 'Game::checkInputCoordinates(const int X, const int Y)'
    if (!_minesHaveBeenSet) {
        this->createMinesAndNums_(x, y);
    }

    if (!(this->isCellVisible_(x, y)) && !(this->isCellMarked_(x, y))) {
        this->makeCellVisible_(x, y);
        ++(this->_numOfVisibleCells);

        if (this->doesCellHaveMine_(x, y)) {
            this->_checkedMine = true;
        } else if (this->numOfMinesAroundCell_(x, y) == 0) {
            this->checkAroundCoordinate_(x, y);
        }
    }
}

void Game::checkAroundCoordinate_(const int x, const int y) {

    assert(x >= 0 && y >= 0 && x < this->_gridWidth && y < this->_gridHeight);

    for (auto const& neighbourCell : this->neighbourCells_(x, y)) {
        this->checkInputCoordinates_(neighbourCell.first, neighbourCell.second);
    }
}

void Game::completeAroundInputCoordinates(const int x, const int y) {

    if (x < 0 || y < 0 || x >= this->_gridWidth || y >= this->_gridHeight) {
        throw std::out_of_range("Game::completeAroundInputCoordinates(const int x, const int y): "
                                "Trying to complete around a cell outside grid.");
    }

    if (!this->isCellVisible_(x, y)) {
        throw std::invalid_argument("Game::completeAroundInputCoordinates(const int x, const int y): "
                                    "Trying to complete around a cell that is not visible.");
    }

    // if cell has a mine
    //   OR the number of marked mines around cell does not match the actual number of mines around cell,
    // do nothing
    // else check unchecked and unmarked cells around given coordinates
    if (!this->doesCellHaveMine_(x, y) &&
        (this->numOfMarkedCellsAroundCell_(x, y) == this->numOfMinesAroundCell_(x, y))) {

        this->checkAroundCoordinate_(x, y);
    }
}

int Game::numOfMarkedCellsAroundCell_(const int x, const int y) const {

    assert(x >= 0 && y >= 0 && x < this->_gridWidth && y < this->_gridHeight);
    int output = 0;
    for (auto const& neighbourCell : this->neighbourCells_(x, y)) {
        if (this->isCellMarked_(x, y)) {
            ++output;
        }
    }
    return output;
}

std::vector<std::pair<int, int>> Game::neighbourCells_(const int x, const int y) const {

    assert(x >= 0 && y >= 0 && x < this->_gridWidth && y < this->_gridHeight);
    std::vector<std::pair<int, int>> neighbours;

    if (x > 0) {
        neighbours.emplace_back(x - 1, y); // left
    }
    if (x < this->_gridWidth - 1) {
        neighbours.emplace_back(x + 1, y); // right
    }
    if (y > 0) {
        neighbours.emplace_back(x, y - 1); // top
    }
    if (y < this->_gridHeight - 1) {
        neighbours.emplace_back(x, y + 1); // bottom
    }
    if (x > 0 && y > 0) {
        neighbours.emplace_back(x - 1, y - 1); // top left
    }
    if (y > 0 && x < this->_gridWidth - 1) {
        neighbours.emplace_back(x + 1, y - 1); // top right
    }
    if (y < this->_gridHeight - 1 && x > 0) {
        neighbours.emplace_back(x - 1, y + 1); // bottom left
    }
    if (x < this->_gridWidth - 1 && y < this->_gridHeight - 1) {
        neighbours.emplace_back(x + 1, y + 1); // bottom right
    }

    return neighbours;
}

int Game::getGridHeight() const { return this->_gridHeight; }

int Game::getGridWidth() const { return this->_gridWidth; }

int Game::getNumOfMines() const { return this->_numOfMines; }

std::ostream& Game::serialise(std::ostream& outStream) const {

    // current serialisation magic and version:
    const std::string CURRENT_SERIALISATION_MAGIC = "Timi's Minesweeper";
    const std::string CURRENT_SERIALISATION_VERSION = "1.0";

    // +-------------------------+
    // | Start of serialisation: |
    // +-------------------------+

    nlohmann::json j;

    // magic and version information:
    j[MAGIC_KEY] = CURRENT_SERIALISATION_MAGIC;
    j[VERSION_KEY] = CURRENT_SERIALISATION_VERSION;

    // current game fields:
    j[CURRENT_GAME_KEY][GRID_HEIGHT_KEY] = this->_gridHeight;
    j[CURRENT_GAME_KEY][GRID_WIDTH_KEY] = this->_gridWidth;
    j[CURRENT_GAME_KEY][NUM_OF_MINES_KEY] = this->_numOfMines;
    j[CURRENT_GAME_KEY][NUM_OF_MARKED_MINES_KEY] = this->_numOfMarkedMines;
    j[CURRENT_GAME_KEY][NUM_OF_WRONGLY_MARKED_CELLS_KEY] = this->_numOfWronglyMarkedCells;
    j[CURRENT_GAME_KEY][NUM_OF_VISIBLE_CELLS_KEY] = this->_numOfVisibleCells;
    j[CURRENT_GAME_KEY][CHECKED_MINE_KEY] = this->_checkedMine;
    j[CURRENT_GAME_KEY][MINES_HAVE_BEEN_SET_KEY] = this->_minesHaveBeenSet;

    // cell data:
    if (this->_gridHeight != 0 && this->_gridWidth != 0) {
        for (int y = 0; y < this->_gridHeight; ++y) {

            nlohmann::json jRowObject;
            jRowObject[ROW_NUMBER_KEY] = y;

            for (int x = 0; x < this->_gridWidth; ++x) {

                nlohmann::json jCellObject;
                jCellObject[COLUMN_NUMBER_KEY] = x;

                jCellObject[CELL_KEY] = this->_cells[y][x]->serialise();

                jRowObject[ROW_CELLS_KEY].push_back(jCellObject);
            }

            j[CURRENT_GAME_KEY][CELLS_KEY].push_back(jRowObject);
        }
    }

    outStream << std::setw(4) << j << std::endl;

    return outStream;
}

std::istream& Game::deserialise(std::istream& inStream) {

    // this method assigns field straight away,
    // hence it cannot be trusted to leave Game in valid state if and error occurs.

    // current serialisation magic and version:
    const std::string CURRENT_SERIALISATION_MAGIC = "Timi's Minesweeper";
    const std::string CURRENT_SERIALISATION_VERSION = "1.0";

    try {

        nlohmann::json j;
        inStream >> j;

        if (j.at(MAGIC_KEY) == CURRENT_SERIALISATION_MAGIC) {
            if (j.at(VERSION_KEY) == CURRENT_SERIALISATION_VERSION) {

                // current game fields:
                int newGridHeight = verifyGridDimension_(j.at(CURRENT_GAME_KEY).at(GRID_HEIGHT_KEY));
                int newGridWidth = verifyGridDimension_(j.at(CURRENT_GAME_KEY).at(GRID_WIDTH_KEY));
                int newNumOfMines =
                    verifyNumOfMines_(j.at(CURRENT_GAME_KEY).at(NUM_OF_MINES_KEY), newGridHeight, newGridWidth);
                int newNumOfMarkedMines =
                    verifyNumOfMarkedMines_(j.at(CURRENT_GAME_KEY).at(NUM_OF_MARKED_MINES_KEY), newNumOfMines);
                int newNumOfWronglyMarkedCells =
                    verifyNumOfWronglyMarkedCells_(j.at(CURRENT_GAME_KEY).at(NUM_OF_WRONGLY_MARKED_CELLS_KEY),
                                                   newGridHeight, newGridWidth, newNumOfMines);
                int newNumOfVisibleCells = verifyNumOfVisibleCells_(j.at(CURRENT_GAME_KEY).at(NUM_OF_VISIBLE_CELLS_KEY),
                                                                    newGridHeight, newGridWidth);
                bool newCheckedMine = j.at(CURRENT_GAME_KEY).at(CHECKED_MINE_KEY);
                bool newMinesHaveBeenSet = j.at(CURRENT_GAME_KEY).at(MINES_HAVE_BEEN_SET_KEY);

                this->_gridHeight = newGridHeight;
                this->_gridWidth = newGridWidth;
                this->_numOfMines = newNumOfMines;
                this->_numOfMarkedMines = newNumOfMarkedMines;
                this->_numOfWronglyMarkedCells = newNumOfWronglyMarkedCells;
                this->_numOfVisibleCells = newNumOfVisibleCells;
                this->_checkedMine = newCheckedMine;
                this->_minesHaveBeenSet = newMinesHaveBeenSet;

                // resize cells/grid to accept their data
                this->resizeCells_(newGridHeight, newGridWidth);

                // cell data:
                if (this->_gridHeight != 0 && this->_gridWidth != 0) {
                    for (auto& jRowObject : j.at(CURRENT_GAME_KEY).at(CELLS_KEY)) {

                        const int Y = jRowObject.at(ROW_NUMBER_KEY);

                        for (auto& jCellObject : jRowObject.at(ROW_CELLS_KEY)) {

                            const int X = jCellObject.at(COLUMN_NUMBER_KEY);

                            this->_cells[Y][X]->deserialise(jCellObject.at(CELL_KEY));
                        }
                    }
                }
            } else {
                throw std::invalid_argument("Game::deserialise(std::istream& inStream): Invalid version number: " +
                                            j.at("version").get<std::string>());
            }
        } else {
            throw std::invalid_argument("Game::deserialise(std::istream& inStream): Invalid magic: " +
                                        j.at("magic").get<std::string>());
        }

    } catch (nlohmann::json::parse_error& ex) {
        throw std::invalid_argument("Game::deserialise(std::istream& inStream): Argument is not valid JSON.\n\t" +
                                    std::string(ex.what()));
    } catch (nlohmann::json::type_error& ex) {
        throw std::invalid_argument("Game::deserialise(std::istream& inStream): Argument is not a JSON object.\n\t" +
                                    std::string(ex.what()));
    } catch (nlohmann::json::out_of_range& ex) {
        throw std::invalid_argument("Game::deserialise(std::istream& inStream): Argument key does not exist.\n\t" +
                                    std::string(ex.what()));
    }

    return inStream;
}

// static method
void Game::setDefaultRandom(IRandom* defaultRandom) { Game::defaultRandom = defaultRandom; }

// static method
int Game::maxNumOfMines(const int gridH, const int gridW) {

    if (gridH < 0 || gridW < 0) {
        throw std::out_of_range("Game::maxNumOfMines(int gridH, int gridW): "
                                "Trying to check maximum number of mines for a negative sized grid.");
    }

    return std::max(gridH * gridW - MAX_NUMBER_OF_CELLS_AROUND_MINE - 1, 0);
}

// static method
double Game::maxProportionOfMines(const int gridH, const int gridW) {

    if (gridH < 0 || gridW < 0) {
        throw std::out_of_range("Game::maxProportionOfMines(int gridHeight, int gridWidth): "
                                "Trying to check maximum proportion of mines for a negative sized grid.");
    }

    double output = 0;

    int numberOfCells = gridH * gridW;
    if (numberOfCells != 0) {
        output = static_cast<double>(maxNumOfMines(gridH, gridW)) / numberOfCells;
    }

    return output;
}

// static method
int Game::minNumOfMines() { return 0; }

// static method
int Game::minNumOfMines(const int gridH, const int gridW) {

    if (gridH < 0 || gridW < 0) {
        throw std::out_of_range("Game::minNumOfMines(int gridH, int gridW): "
                                "Trying to check minimum number of mines for a negative sized grid.");
    }

    return minNumOfMines();
}

// static method
double Game::minProportionOfMines() { return minNumOfMines(); }

// static method
double Game::minProportionOfMines(const int gridH, const int gridW) {

    if (gridH < 0 || gridW < 0) {
        throw std::out_of_range("Game::minProportionOfMines(const int gridH, const int gridW): "
                                "Trying to check minimum number of mines for a negative sized grid.");
    }

    double output = 0;

    int numberOfCells = gridH * gridW;
    if (numberOfCells != 0) {
        output = static_cast<double>(minNumOfMines(gridH, gridW)) / numberOfCells;
    }

    return output;
}

} // namespace minesweeper