#include <cassert>  // assert
#include <iomanip>   // std::setw
#include <stdexcept> // std::invalid_argument

#include <nlohmann/json.hpp> // nlohmann::json

#include <minesweeper/cell.h>

namespace minesweeper {

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

    nlohmann::json j;

    // version information
    j["cellVersion"] = "1.0";

    // cell fields
    j["_isVisible"] = this->_isVisible;
    j["_hasMine"] = this->_hasMine;
    j["_isMarked"] = this->_isMarked;
    j["_numOfMinesAround"] = this->_numOfMinesAround;

    outStream << std::setw(4) << j << std::endl;

    return outStream;
}

nlohmann::json Cell::serialise() const {

    nlohmann::json j;

    // version information
    j["cellVersion"] = "1.0";

    // cell fields
    j["_isVisible"] = this->_isVisible;
    j["_hasMine"] = this->_hasMine;
    j["_isMarked"] = this->_isMarked;
    j["_numOfMinesAround"] = this->_numOfMinesAround;
    return j;
}

std::istream& Cell::deserialise(std::istream& inStream) {

    try {

        nlohmann::json j;
        inStream >> j;

        if (j.at("cellVersion") == "1.0") {

            // cell fields
            this->_isVisible = j.at("_isVisible");
            this->_hasMine = j.at("_hasMine");
            this->_isMarked = j.at("_isMarked");
            this->_numOfMinesAround = j.at("_numOfMinesAround");
        }

    } catch (nlohmann::json::parse_error& ex) {
        throw std::invalid_argument("Cell::deserialise(std::istream& inStream): Argument is not valid JSON.\n\t" +
                                    std::string(ex.what()));
    } catch (nlohmann::json::type_error& ex) {
        throw std::invalid_argument("Cell::deserialise(std::istream& inStream): Argument is not a JSON object.\n\t" +
                                    std::string(ex.what()));
    } catch (nlohmann::json::out_of_range& ex) {
        throw std::invalid_argument("Cell::deserialise(std::istream& inStream): Argument key does not exist.\n\t" +
                                    std::string(ex.what()));
    }

    return inStream;
}

void Cell::deserialise(nlohmann::json& j) {

    if (j.at("cellVersion") == "1.0") {

        // cell fields
        this->_isVisible = j.at("_isVisible");
        this->_hasMine = j.at("_hasMine");
        this->_isMarked = j.at("_isMarked");
        this->_numOfMinesAround = j.at("_numOfMinesAround");
    }
}

} // namespace minesweeper