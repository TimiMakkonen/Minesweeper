#ifndef MINESWEEPER_CELL_H
#define MINESWEEPER_CELL_H

#include <iostream> // std::istream, std::ostream

#include <nlohmann/json.hpp> // nlohmann::json

#include <minesweeper/visual_minesweeper_cell.h>

namespace minesweeper {

class Cell {
  private:
    bool _isVisible = false;
    bool _hasMine = false;
    bool _isMarked = false;
    int _numOfMinesAround = 0;

  public:
    void reset(bool keepMineInformation);

    // cell information methods:
    bool isVisible() const;
    bool hasMine() const;
    bool isMarked() const;
    int numOfMinesAround() const;

    // cell mine creation methods:
    void putMine();
    void incrNumOfMinesAround();

    // cell checking method:
    void makeVisible();

    // cell marking methods:
    void markCell();
    void unmarkCell();

    // save cell:
    std::ostream& serialise(std::ostream& outStream) const;
    nlohmann::json serialise() const;

    // load cell:
    std::istream& deserialise(std::istream& inStream);
    void deserialise(nlohmann::json& j);

    // visualise to 'VisualMinesweeperCell' method
    VisualMinesweeperCell visualise() const;

  private:
    nlohmann::json serialise_() const;

    void deserialise_(nlohmann::json& j);
};

} // namespace minesweeper

#endif // MINESWEEPER_CELL_H