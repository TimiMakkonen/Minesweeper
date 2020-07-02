#include <algorithm> // std::max
#include <cassert>   // assert
#include <iomanip>   // std::setw
#include <memory>    // std::unique_ptr
#include <numeric>   // std::iota
#include <sstream>   // std::ostringstream
#include <stdexcept> // std::out_of_range
#include <vector>    // std::vector

#include <nlohmann/json.hpp> // nlohmann::json

#include <minesweeper/cell.h>
#include <minesweeper/game.h>
#include <minesweeper/visual_minesweeper_cell.h>

namespace minesweeper {

// initialisation of static fields:
IRandom* Game::defaultRandom = nullptr;

Game::Game() = default;

Game::Game(int gridSize, int numOfMines, IRandom* random) : Game(gridSize, gridSize, numOfMines, random) {}

Game::Game(int gridHeight, int gridWidth, int numOfMines, IRandom* random)
    : gridHeight{verifyGridDimension(gridHeight)},                     // throws
      gridWidth{verifyGridDimension(gridWidth)},                       // throws
      numOfMines{verifyNumOfMines(numOfMines, gridHeight, gridWidth)}, // throws
      cells{initCells(this->gridHeight, this->gridWidth)}, random{random} {}

Game::Game(int gridSize, double proportionOfMines, IRandom* random)
    : Game(gridSize, gridSize, proportionOfMines, random) {}

Game::Game(int gridHeight, int gridWidth, double proportionOfMines, IRandom* random)
    : gridHeight{verifyGridDimension(gridHeight)}, // throws
      gridWidth{verifyGridDimension(gridWidth)},
      numOfMines{verifyNumOfMines(
          static_cast<int>(verifyProportionOfMines(proportionOfMines, gridHeight, gridWidth) * gridHeight * gridWidth),
          gridHeight, gridWidth)}, // throws
      cells{initCells(this->gridHeight, this->gridWidth)},
      random{random} {}

// required by to solve "error C2027: use of undefined type"
// in short, std::unique_ptr requires destructor to be defined here
// Check more info on this:
// https://stackoverflow.com/questions/40383395/use-of-undefined-type-with-unique-ptr-to-forward-declared-class-and-defaulted
// https://stackoverflow.com/questions/6012157/is-stdunique-ptrt-required-to-know-the-full-definition-of-t
// https://stackoverflow.com/questions/13414652/forward-declaration-with-unique-ptr
Game::~Game() = default;

// static method
int Game::verifyGridDimension(int gridDimension) {

    if (gridDimension < 0) {
        throw std::out_of_range(
            "Game::verifyGridDimension(int gridDimension): Trying to create a grid with negative (" +
            std::to_string(gridDimension) + ") grid dimension.");
    }

    return gridDimension;
}

// static method
int Game::verifyNumOfMines(int numOfMines, int gridHeight, int gridWidth) {

    if (numOfMines > maxNumOfMines(gridHeight, gridWidth)) {
        throw std::out_of_range("Game::verifyNumOfMines(int numOfMines, int gridHeight, int gridWidth): "
                                "Trying to create a grid with too many (" +
                                std::to_string(numOfMines) + ") mines.");
    }

    if (numOfMines < minNumOfMines(gridHeight, gridWidth)) {
        throw std::out_of_range("Game::verifyNumOfMines(int numOfMines, int gridHeight, int gridWidth): "
                                "Trying to create a grid with too few (" +
                                std::to_string(numOfMines) + ") mines.");
    }

    return numOfMines;
}

// static method
double Game::verifyProportionOfMines(double proportionOfMines, int gridHeight, int gridWidth) {

    if (proportionOfMines > maxProportionOfMines(gridHeight, gridWidth)) {
        throw std::out_of_range(
            "Game::verifyProportionOfMines(double proportionOfMines, int gridHeight, int gridWidth): "
            "Trying to create a grid with too big proportion (" +
            std::to_string(proportionOfMines) + ") of mines.");
    }

    if (proportionOfMines < minProportionOfMines(gridHeight, gridWidth)) {
        throw std::out_of_range(
            "Game::verifyProportionOfMines(double proportionOfMines, int gridHeight, int gridWidth): "
            "Trying to create a grid with too small proportion (" +
            std::to_string(proportionOfMines) + ") of mines.");
    }

    return proportionOfMines;
}

// static method
int Game::verifyNumOfMarkedMines(int numOfMarkedMines, int numOfMines) {

    if (numOfMarkedMines > numOfMines) {
        throw std::out_of_range("Game::verifyNumOfMarkedMines(int numOfMarkedMines, int numOfMines): "
                                "Trying to create a grid with too many (" +
                                std::to_string(numOfMarkedMines) + ") marked mines.");
    }

    if (numOfMarkedMines < 0) {
        throw std::out_of_range("Game::verifyNumOfMarkedMines(int numOfMarkedMines, int numOfMines): "
                                "Trying to create a grid with too few (" +
                                std::to_string(numOfMarkedMines) + ") marked mines.");
    }

    return numOfMarkedMines;
}

// static method
int Game::verifyNumOfWronglyMarkedCells(int numOfWronglyMarkedCells, int gridHeight, int gridWidth, int numOfMines) {

    int numOfMinelessCells = gridHeight * gridWidth - numOfMines;

    if (numOfWronglyMarkedCells > numOfMinelessCells) {
        throw std::out_of_range("Game::verifyNumOfWronglyMarkedCells("
                                "int numOfWronglyMarkedCells, int gridHeight, int gridWidth, int numOfMines): "
                                "Trying to create a grid with too many (" +
                                std::to_string(numOfWronglyMarkedCells) + ") wrongly marked cells.");
    }

    if (numOfWronglyMarkedCells < 0) {
        throw std::out_of_range("Game::verifyNumOfWronglyMarkedCells("
                                "int numOfWronglyMarkedCells, int gridHeight, int gridWidth, int numOfMines): "
                                "Trying to create a grid with too few (" +
                                std::to_string(numOfWronglyMarkedCells) + ") wrongly marked cells.");
    }

    return numOfWronglyMarkedCells;
}

// static method
int Game::verifyNumOfVisibleCells(int numOfVisibleCells, int gridHeight, int gridWidth) {

    if (numOfVisibleCells > gridHeight * gridWidth) {
        throw std::out_of_range("Game::verifyNumOfVisibleCells(int numOfVisibleCells, int gridHeight, int gridWidth): "
                                "Trying to create a grid with too many (" +
                                std::to_string(numOfVisibleCells) + ") visible cells.");
    }

    if (numOfVisibleCells < 0) {
        throw std::out_of_range("Game::verifyNumOfVisibleCells(int numOfVisibleCells, int gridHeight, int gridWidth): "
                                "Trying to create a grid with too few (" +
                                std::to_string(numOfVisibleCells) + ") visible cells.");
    }

    return numOfVisibleCells;
}

// static method
// consider combining with Game::resizeCells()
std::vector<std::vector<std::unique_ptr<Cell>>> Game::initCells(const int gridH, const int gridW) {

    assert(gridH >= 0 && gridW >= 0);

    std::vector<std::vector<std::unique_ptr<Cell>>> outputCellMatrix;
    outputCellMatrix.reserve(gridH);
    for (int y = 0; y < gridH; ++y) {
        std::vector<std::unique_ptr<Cell>> outputCellRow;
        outputCellRow.reserve(gridW);
        for (int x = 0; x < gridW; ++x) {
            outputCellRow.emplace_back(std::make_unique<Cell>());
        }
        outputCellMatrix.emplace_back(std::move(outputCellRow));
    }
    return outputCellMatrix;
}

// consider combining with Game::initCells()
void Game::resizeCells(const int gridH, const int gridW) {

    assert(gridH >= 0 && gridW >= 0);

    this->cells.reserve(gridH);
    this->cells.resize(gridH);
    for (auto& cellRow : cells) {

        cellRow.reserve(gridW);
        if (cellRow.size() < gridW) {
            for (size_t i = cellRow.size(); i < gridW; ++i) {
                cellRow.emplace_back(std::make_unique<Cell>());
            }
        }
        cellRow.resize(gridW);
    }
}

void Game::createMinesAndNums(const int initChosenX, const int initChosenY) {

    if (initChosenX < 0 || initChosenY < 0 || initChosenX >= this->gridWidth || initChosenY >= this->gridHeight) {
        throw std::out_of_range("Game::createMinesAndNums(const int initChosenX, const int initChosenY): "
                                "Trying to use initial cell (initChosenX, initChosenX) outside grid.");
    }

    if (minesHaveBeenSet) {
        throw std::invalid_argument("Game::createMinesAndNums(const int initChosenX, const int initChosenY): "
                                    "Trying to create mines and numbers for a grid that already has them created.");
    }

    this->createMinesAndNums_(initChosenX, initChosenY);
}

void Game::createMinesAndNums_(const int initChosenX, const int initChosenY) {

    assert(initChosenX >= 0 && initChosenY >= 0 && initChosenX < this->gridWidth && initChosenY < this->gridHeight);

    assert(!minesHaveBeenSet);

    std::vector<int> mineSpots(this->gridWidth * this->gridHeight);
    this->chooseRandomMineCells(mineSpots, initChosenX, initChosenY);

    int X = 0;
    int Y = 0;
    for (int i = 0; i < this->numOfMines; ++i) {

        X = mineSpots[i] % this->gridWidth;
        Y = mineSpots[i] / this->gridWidth;

        this->createMine(X, Y);

        this->incrNumsAroundMine(X, Y);
    }

    this->minesHaveBeenSet = true;
}

void Game::chooseRandomMineCells(std::vector<int>& mineSpots, const int initChosenX, const int initChosenY) const {

    // to populate mineSpots vector with values: 0, 1, 2, ..., gridWidth*gridHeight - 1
    std::iota(mineSpots.begin(), mineSpots.end(), 0);

    // to shuffle this vector
    this->randomizeMineVector(mineSpots);

    // to remove bad gridspots (those on and around chosen initial spot)
    mineSpots.erase(std::remove(mineSpots.begin(), mineSpots.end(), (initChosenY * this->gridWidth + initChosenX)),
                    mineSpots.end());
    if (initChosenX > 0) {
        mineSpots.erase(
            std::remove(mineSpots.begin(), mineSpots.end(), (initChosenY * this->gridWidth + initChosenX - 1)),
            mineSpots.end()); // left
    }
    if (initChosenX < this->gridWidth - 1) {
        mineSpots.erase(
            std::remove(mineSpots.begin(), mineSpots.end(), (initChosenY * this->gridWidth + initChosenX + 1)),
            mineSpots.end()); // right
    }
    if (initChosenY > 0) {
        mineSpots.erase(
            std::remove(mineSpots.begin(), mineSpots.end(), ((initChosenY - 1) * this->gridWidth + initChosenX)),
            mineSpots.end()); // top
    }
    if (initChosenY < this->gridHeight - 1) {
        mineSpots.erase(
            std::remove(mineSpots.begin(), mineSpots.end(), ((initChosenY + 1) * this->gridWidth + initChosenX)),
            mineSpots.end()); // bottom
    }
    if (initChosenY > 0 && initChosenX > 0) {
        mineSpots.erase(
            std::remove(mineSpots.begin(), mineSpots.end(), ((initChosenY - 1) * this->gridWidth + initChosenX - 1)),
            mineSpots.end()); // top left
    }
    if (initChosenY > 0 && initChosenX < this->gridWidth - 1) {
        mineSpots.erase(
            std::remove(mineSpots.begin(), mineSpots.end(), ((initChosenY - 1) * this->gridWidth + initChosenX + 1)),
            mineSpots.end()); // top right
    }
    if (initChosenY < this->gridHeight - 1 && initChosenX > 0) {
        mineSpots.erase(
            std::remove(mineSpots.begin(), mineSpots.end(), ((initChosenY + 1) * this->gridWidth + initChosenX - 1)),
            mineSpots.end()); // bottom left
    }
    if (initChosenY < this->gridHeight - 1 && initChosenX < this->gridWidth - 1) {
        mineSpots.erase(
            std::remove(mineSpots.begin(), mineSpots.end(), ((initChosenY + 1) * this->gridWidth + initChosenX + 1)),
            mineSpots.end()); // bottom right
    }
}

void Game::randomizeMineVector(std::vector<int>& mineSpots) const {

    if (this->random != nullptr) {
        this->random->shuffleVector(mineSpots);
    } else {
        if (Game::defaultRandom != nullptr) {
            Game::defaultRandom->shuffleVector(mineSpots);
        } else {
            throw std::invalid_argument("Game::randomizeMineVector(std::vector<int>& mineSpots): "
                                        "Neither field 'random' nor static field 'defaultRandom' is initialised.");
        }
    }
}

void Game::createMine(const int X, const int Y) {

    assert(X >= 0 && Y >= 0 && X < this->gridWidth && Y < this->gridHeight);
    this->cells[Y][X]->putMine();
}

void Game::incrNumsAroundMine(const int X, const int Y) {

    assert(X >= 0 && Y >= 0 && X < this->gridWidth && Y < this->gridHeight);
    if (X > 0) {
        this->cells[Y][X - 1]->incrNumOfMinesAround(); // left
    }
    if (X < this->gridWidth - 1) {
        this->cells[Y][X + 1]->incrNumOfMinesAround(); // right
    }
    if (Y > 0) {
        this->cells[Y - 1][X]->incrNumOfMinesAround(); // top
    }
    if (Y < this->gridHeight - 1) {
        this->cells[Y + 1][X]->incrNumOfMinesAround(); // bottom
    }
    if (Y > 0 && X > 0) {
        this->cells[Y - 1][X - 1]->incrNumOfMinesAround(); // top left
    }
    if (Y > 0 && X < this->gridWidth - 1) {
        this->cells[Y - 1][X + 1]->incrNumOfMinesAround(); // top right
    }
    if (Y < this->gridHeight - 1 && X > 0) {
        this->cells[Y + 1][X - 1]->incrNumOfMinesAround(); // bottom left
    }
    if (Y < this->gridHeight - 1 && X < this->gridWidth - 1) {
        this->cells[Y + 1][X + 1]->incrNumOfMinesAround(); // bottom right
    }
}

void Game::reset(bool keepCreatedMines) { this->reset_(keepCreatedMines); }

void Game::reset_(bool keepCreatedMines) {

    // reset game fields
    this->numOfMarkedMines = 0;
    this->numOfWronglyMarkedCells = 0;
    this->numOfVisibleCells = 0;
    this->_checkedMine = false;

    if (!keepCreatedMines) {
        this->minesHaveBeenSet = false;
    }

    // reset cells
    for (auto& cellRow : this->cells) {
        for (auto& cell : cellRow) {

            cell->reset(keepCreatedMines);
        }
    }
}

void Game::newGame(int gridSize, int numOfMines) { this->newGame(gridSize, gridSize, numOfMines); }

void Game::newGame(int gridHeight, int gridWidth, int numOfMines) {

    // parameter verification
    verifyGridDimension(gridHeight);
    verifyGridDimension(gridWidth);
    verifyNumOfMines(numOfMines, gridHeight, gridWidth);

    this->reset_(false);
    this->resizeCells(gridHeight, gridWidth);
    this->gridHeight = gridHeight;
    this->gridWidth = gridWidth;
    this->numOfMines = numOfMines;
}

void Game::newGame(int gridSize, double proportionOfMines) { this->newGame(gridSize, gridSize, proportionOfMines); }

void Game::newGame(int gridHeight, int gridWidth, double proportionOfMines) {

    // parameter verification
    verifyGridDimension(gridHeight);
    verifyGridDimension(gridWidth);
    verifyProportionOfMines(proportionOfMines, gridHeight, gridWidth);
    int newNumOfMines = static_cast<int>(proportionOfMines * gridHeight * gridWidth);
    verifyNumOfMines(newNumOfMines, gridHeight, gridWidth);

    this->reset_(false);
    this->resizeCells(gridHeight, gridWidth);
    this->gridHeight = gridHeight;
    this->gridWidth = gridWidth;
    this->numOfMines = newNumOfMines;
}

// to mark (or unmark) given coordinates, and keeping track of marked and wrongly marked mines
void Game::markInputCoordinates(const int X, const int Y) {

    if (X < 0 || Y < 0 || X >= this->gridWidth || Y >= this->gridHeight) {
        throw std::out_of_range(
            "Game::markInputCoordinates(const int X, const int Y): Trying to mark cell outside grid.");
    }

    if (!minesHaveBeenSet) {
        throw std::invalid_argument("Game::markInputCoordinates(const int X, const int Y): "
                                    "Trying to mark a cell before mines hava been initialised. \n\t"
                                    "(Initialise mines by calling: "
                                    "createMinesAndNums(const int initChosenX, const int initChosenY) or "
                                    "checkInputCoordinates(const int X, const int Y).)");
    }
    if (!this->isCellVisible_(X, Y)) {
        if (this->isCellMarked_(X, Y)) {
            this->unmarkCell(X, Y);

            if (this->doesCellHaveMine_(X, Y)) {
                --(this->numOfMarkedMines);
            } else {
                --(this->numOfWronglyMarkedCells);
            }
        } else {
            if (!this->isCellVisible_(X, Y)) {

                this->markCell(X, Y);

                if (this->doesCellHaveMine_(X, Y)) {
                    ++(this->numOfMarkedMines);
                } else {
                    ++(this->numOfWronglyMarkedCells);
                }
            }
        }
    }
}

bool Game::allMinesMarked() const { return this->numOfMines == this->numOfMarkedMines; }

bool Game::noNonMinesMarked() const { return this->numOfWronglyMarkedCells == 0; }

bool Game::allNonMinesVisible() const {

    return this->numOfVisibleCells + this->numOfMines == this->gridWidth * this->gridHeight && !this->_checkedMine;
}

bool Game::playerHasWon() const {

    bool playerWon = false;

    if (!this->playerHasLost_()) {
        if (this->allNonMinesVisible() || (this->allMinesMarked() && this->noNonMinesMarked())) {
            playerWon = true;
        }
    }
    return playerWon;
}

bool Game::playerHasLost() const { return this->playerHasLost_(); }

bool Game::playerHasLost_() const { return this->checkedMine(); }

bool Game::checkedMine() const { return this->_checkedMine; }

bool Game::isCellVisible(const int X, const int Y) const {

    if (X < 0 || Y < 0 || X >= this->gridWidth || Y >= this->gridHeight) {
        throw std::out_of_range(
            "Game::isCellVisible(const int X, const int Y): Trying to check if a cell outside the grid is visible.");
    }
    return this->isCellVisible_(X, Y);
}

bool Game::isCellVisible_(const int X, const int Y) const {

    assert(X >= 0 && Y >= 0 && X < this->gridWidth && Y < this->gridHeight);

    return this->cells[Y][X]->isVisible();
}

bool Game::doesCellHaveMine(const int X, const int Y) const {

    if (X < 0 || Y < 0 || X >= this->gridWidth || Y >= this->gridHeight) {
        throw std::out_of_range(
            "Game::doesCellHaveMine(const int X, const int Y): Trying to check if a cell outside the grid has a mine.");
    }
    return this->doesCellHaveMine_(X, Y);
}

bool Game::doesCellHaveMine_(const int X, const int Y) const {

    assert(X >= 0 && Y >= 0 && X < this->gridWidth && Y < this->gridHeight);

    return this->cells[Y][X]->hasMine();
}

bool Game::isCellMarked(const int X, const int Y) const {

    if (X < 0 || Y < 0 || X >= this->gridWidth || Y >= this->gridHeight) {
        throw std::out_of_range(
            "Game::isCellMarked(const int X, const int Y): Trying to check if a cell outside the grid is marked.");
    }
    return this->isCellMarked_(X, Y);
}

bool Game::isCellMarked_(const int X, const int Y) const {

    assert(X >= 0 && Y >= 0 && X < this->gridWidth && Y < this->gridHeight);

    return this->cells[Y][X]->isMarked();
}

int Game::numOfMinesAroundCell(const int X, const int Y) const {

    if (X < 0 || Y < 0 || X >= this->gridWidth || Y >= this->gridHeight) {
        throw std::out_of_range("Game::numOfMinesAroundCell(const int X, const int Y): "
                                "Trying to check number of mines around a cell outside the grid.");
    }
    return this->numOfMinesAroundCell_(X, Y);
}

int Game::numOfMinesAroundCell_(const int X, const int Y) const {

    assert(X >= 0 && Y >= 0 && X < this->gridWidth && Y < this->gridHeight);

    return this->cells[Y][X]->numOfMinesAround();
}

void Game::makeCellVisible(const int X, const int Y) {
    assert(X >= 0 && Y >= 0 && X < this->gridWidth && Y < this->gridHeight);
    this->cells[Y][X]->makeVisible();
}

void Game::markCell(const int X, const int Y) {
    assert(X >= 0 && Y >= 0 && X < this->gridWidth && Y < this->gridHeight);
    this->cells[Y][X]->markCell();
}

void Game::unmarkCell(const int X, const int Y) {
    assert(X >= 0 && Y >= 0 && X < this->gridWidth && Y < this->gridHeight);
    this->cells[Y][X]->unmarkCell();
}

VisualMinesweeperCell Game::visualiseCell_(const int X, const int Y) const {
    assert(X >= 0 && Y >= 0 && X < this->gridWidth && Y < this->gridHeight);
    return this->cells[Y][X]->visualise();
}

// to check user given coordinates, and make it visible
void Game::checkInputCoordinates(const int X, const int Y) {

    if (X < 0 || Y < 0 || X >= this->gridWidth || Y >= this->gridHeight) {
        throw std::out_of_range(
            "Game::checkInputCoordinates(const int X, const int Y): Trying to check cell outside grid.");
    }

    this->checkInputCoordinates_(X, Y);
}

void Game::checkInputCoordinates_(const int X, const int Y) {

    assert(X >= 0 && Y >= 0 && X < this->gridWidth && Y < this->gridHeight);

    // could maybe be moved to public 'Game::checkInputCoordinates(const int X, const int Y)'
    if (!minesHaveBeenSet) {
        this->createMinesAndNums_(X, Y);
    }

    if (!(this->isCellVisible_(X, Y)) && !(this->isCellMarked_(X, Y))) {
        this->makeCellVisible(X, Y);
        ++(this->numOfVisibleCells);

        if (this->doesCellHaveMine_(X, Y)) {
            this->_checkedMine = true;
        } else if (this->numOfMinesAroundCell_(X, Y) == 0) {
            this->checkAroundCoordinate(X, Y);
        }
    }
}

void Game::checkAroundCoordinate(const int X, const int Y) {

    assert(X >= 0 && Y >= 0 && X < this->gridWidth && Y < this->gridHeight);
    if (X > 0) {
        this->checkInputCoordinates_(X - 1, Y); // left
    }
    if (X < this->gridWidth - 1) {
        this->checkInputCoordinates_(X + 1, Y); // right
    }
    if (Y > 0) {
        this->checkInputCoordinates_(X, Y - 1); // top
    }
    if (Y < this->gridHeight - 1) {
        this->checkInputCoordinates_(X, Y + 1); // bottom
    }
    if (X > 0 && Y > 0) {
        this->checkInputCoordinates_(X - 1, Y - 1); // top left
    }
    if (Y > 0 && X < this->gridWidth - 1) {
        this->checkInputCoordinates_(X + 1, Y - 1); // top right
    }
    if (Y < this->gridHeight - 1 && X > 0) {
        this->checkInputCoordinates_(X - 1, Y + 1); // bottom left
    }
    if (X < this->gridWidth - 1 && Y < this->gridHeight - 1) {
        this->checkInputCoordinates_(X + 1, Y + 1); // bottom right
    }
}

int Game::getGridHeight() const { return this->gridHeight; }

int Game::getGridWidth() const { return this->gridWidth; }

int Game::getNumOfMines() const { return this->numOfMines; }

std::ostream& Game::serialise(std::ostream& outStream) const {

    nlohmann::json j;

    // magic and version information:
    j["magic"] = "Timi's Minesweeper";
    j["version"] = "1.0";

    // current game fields:
    j["currentGame"]["gridHeight"] = this->gridHeight;
    j["currentGame"]["gridWidth"] = this->gridWidth;
    j["currentGame"]["numOfMines"] = this->numOfMines;
    j["currentGame"]["numOfMarkedMines"] = this->numOfMarkedMines;
    j["currentGame"]["numOfWronglyMarkedCells"] = this->numOfWronglyMarkedCells;
    j["currentGame"]["numOfVisibleCells"] = this->numOfVisibleCells;
    j["currentGame"]["_checkedMine"] = this->_checkedMine;
    j["currentGame"]["minesHaveBeenSet"] = this->minesHaveBeenSet;

    // cell data:
    if (this->gridHeight != 0 && this->gridWidth != 0) {
        for (int y = 0; y < this->gridHeight; ++y) {

            nlohmann::json jRowObject;
            jRowObject["rowNumber"] = y;

            for (int x = 0; x < this->gridWidth; ++x) {

                nlohmann::json jCellObject;
                jCellObject["columnNumber"] = x;

                jCellObject["cell"] = cells[y][x]->serialise();

                jRowObject["rowCells"].push_back(jCellObject);
            }

            j["currentGame"]["cells"].push_back(jRowObject);
        }
    }

    outStream << std::setw(4) << j << std::endl;

    return outStream;
}

std::istream& Game::deserialise(std::istream& inStream) {

    // this method assigns field straight away,
    // hence it cannot be trusted to leave Game in valid state if and error occurs.

    try {

        nlohmann::json j;
        inStream >> j;

        if (j.at("magic") == "Timi's Minesweeper") {
            if (j.at("version") == "1.0") {

                // current game fields:
                int newGridHeight = verifyGridDimension(j.at("currentGame").at("gridHeight"));
                int newGridWidth = verifyGridDimension(j.at("currentGame").at("gridWidth"));
                int newNumOfMines = verifyNumOfMines(j.at("currentGame").at("numOfMines"), newGridHeight, newGridWidth);
                int newNumOfMarkedMines =
                    verifyNumOfMarkedMines(j.at("currentGame").at("numOfMarkedMines"), newNumOfMines);
                int newNumOfWronglyMarkedCells = verifyNumOfWronglyMarkedCells(
                    j.at("currentGame").at("numOfWronglyMarkedCells"), newGridHeight, newGridWidth, newNumOfMines);
                int newNumOfVisibleCells =
                    verifyNumOfVisibleCells(j.at("currentGame").at("numOfVisibleCells"), newGridHeight, newGridWidth);
                bool new_checkedMine = j.at("currentGame").at("_checkedMine");
                bool newMinesHaveBeenSet = j.at("currentGame").at("minesHaveBeenSet");

                this->gridHeight = newGridHeight;
                this->gridWidth = newGridWidth;
                this->numOfMines = newNumOfMines;
                this->numOfMarkedMines = newNumOfMarkedMines;
                this->numOfWronglyMarkedCells = newNumOfWronglyMarkedCells;
                this->numOfVisibleCells = newNumOfVisibleCells;
                this->_checkedMine = new_checkedMine;
                this->minesHaveBeenSet = newMinesHaveBeenSet;

                // resize cells/grid to accept their data
                this->resizeCells(newGridHeight, newGridWidth);

                // cell data:
                if (this->gridHeight != 0 && this->gridWidth != 0) {
                    for (auto& jRowObject : j.at("currentGame").at("cells")) {

                        const int y = jRowObject.at("rowNumber");

                        for (auto& jCellObject : jRowObject.at("rowCells")) {

                            const int x = jCellObject.at("columnNumber");

                            cells[y][x]->deserialise(jCellObject.at("cell"));
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

    return static_cast<double>(maxNumOfMines(gridH, gridW)) / (gridH * gridW);
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

    return static_cast<double>(minNumOfMines(gridH, gridW)) / (gridH * gridW);
}

} // namespace minesweeper