#include <cassert>   // assert
#include <iomanip>   // std::setw
#include <iostream>  // std::istream, std::ostream
#include <stdexcept> // std::invalid_argument
#include <string>    // std::string

#include <json.hpp> // nlohmann::json

#include <minesweeper/cell.h>
#include <minesweeper/visual_minesweeper_cell.h>

namespace minesweeper {

// +----------------------------------+
// | initialisation of static fields: |
// +----------------------------------+

// serialisation keys:
const char* Cell::CELL_VERSION_KEY = "cellVersion";
// basic serialisation structure keys:
const char* Cell::IS_VISIBLE_KEY = "_isVisible";
const char* Cell::HAS_MINE_KEY = "_hasMine";
const char* Cell::IS_MARKED_KEY = "_isMarked";
const char* Cell::NUM_OF_MINES_AROUND_KEY = "_numOfMinesAround";

// +----------+
// | methods: |
// +----------+

bool Cell::isVisible() const { return this->_isVisible; }

bool Cell::hasMine() const { return this->_hasMine; }

bool Cell::isMarked() const { return this->_isMarked; }

int Cell::numOfMinesAround() const { return this->_numOfMinesAround; }

void Cell::incrNumOfMinesAround() {

    ++(this->_numOfMinesAround);
    assert(this->_numOfMinesAround >= 0 && this->_numOfMinesAround < 9);
}

void Cell::makeVisible() { this->_isVisible = true; }

void Cell::putMine() { this->_hasMine = true; }

void Cell::markCell() {

    assert(this->_isMarked != true);
    this->_isMarked = true;
}

void Cell::unmarkCell() {

    assert(this->_isMarked != false);
    this->_isMarked = false;
}

void Cell::reset(bool keepMineInformation) {

    this->_isVisible = false;
    this->_isMarked = false;

    if (!keepMineInformation) {
        this->_hasMine = false;
        this->_numOfMinesAround = 0;
    }
}

std::ostream& Cell::serialise(std::ostream& outStream) const {

    auto j{this->serialise_()};

    outStream << std::setw(4) << j << std::endl;

    return outStream;
}

nlohmann::json Cell::serialise() const { return this->serialise_(); }

nlohmann::json Cell::serialise_() const {

    // current serialisation keys:
    const std::string CURRENT_CELL_VERSION = "1.0";

    nlohmann::json j;

    // version information
    j[CELL_VERSION_KEY] = CURRENT_CELL_VERSION;

    // cell fields
    j[IS_VISIBLE_KEY] = this->_isVisible;
    j[HAS_MINE_KEY] = this->_hasMine;
    j[IS_MARKED_KEY] = this->_isMarked;
    j[NUM_OF_MINES_AROUND_KEY] = this->_numOfMinesAround;

    return j;
}

std::istream& Cell::deserialise(std::istream& inStream) {

    try {

        nlohmann::json j;
        inStream >> j;

        this->deserialise_(j);

    } catch (nlohmann::json::parse_error& ex) {
        throw std::invalid_argument("Cell::deserialise(std::istream& inStream): Argument is not valid JSON.\n\t" +
                                    std::string(ex.what()));
    }

    return inStream;
}

void Cell::deserialise(const nlohmann::json& j) { this->deserialise_(j); }

void Cell::deserialise_(const nlohmann::json& j) {

    // current serialisation keys:
    const std::string CURRENT_CELL_VERSION = "1.0";

    try {

        if (j.at(CELL_VERSION_KEY) == CURRENT_CELL_VERSION) {

            // cell fields
            this->_isVisible = j.at(IS_VISIBLE_KEY);
            this->_hasMine = j.at(HAS_MINE_KEY);
            this->_isMarked = j.at(IS_MARKED_KEY);
            this->_numOfMinesAround = j.at(NUM_OF_MINES_AROUND_KEY);
        }

    } catch (nlohmann::json::type_error& ex) {
        throw std::invalid_argument("Cell::deserialise_(nlohmann::json& j): Argument is not a JSON object.\n\t" +
                                    std::string(ex.what()));
    } catch (nlohmann::json::out_of_range& ex) {
        throw std::invalid_argument("Cell::deserialise_(nlohmann::json& j): Argument key does not exist.\n\t" +
                                    std::string(ex.what()));
    }
}

VisualMinesweeperCell Cell::visualise() const {

    VisualMinesweeperCell output;
    if (this->_isMarked) {
        output = VisualMinesweeperCell::MARKED;
    } else if (!this->_isVisible) {
        output = VisualMinesweeperCell::UNCHECKED;
    } else {
        if (this->_hasMine) {
            output = VisualMinesweeperCell::MINE;
        } else {
            output = static_cast<VisualMinesweeperCell>(this->_numOfMinesAround);
        }
    }

    return output;
}

VisualMinesweeperCell Cell::visualiseSolution() const {

    VisualMinesweeperCell output;
    if (this->_hasMine) {
        output = VisualMinesweeperCell::MINE;
    } else {
        output = static_cast<VisualMinesweeperCell>(this->_numOfMinesAround);
    }

    return output;
}

} // namespace minesweeper