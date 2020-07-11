#ifndef MINESWEEPER_CELL_H
#define MINESWEEPER_CELL_H

#include <iostream> // std::istream, std::ostream

#include <json.hpp> // nlohmann::json

#include <minesweeper/visual_minesweeper_cell.h>

namespace minesweeper {

class Cell {
  private:
    // +---------+
    // | fields: |
    // +---------+

    bool _isVisible = false;
    bool _hasMine = false;
    bool _isMarked = false;
    int _numOfMinesAround = 0;

  public:
    // +-----------------+
    // | public methods: |
    // +-----------------+

    // constructors:
    Cell() = default;
    Cell(const Cell& other) = default;
    Cell(Cell&& other) = default;

    // default destructor:
    ~Cell() = default;

    // assignment operators:
    Cell& operator=(const Cell& other) = default;
    Cell& operator=(Cell&& other) = default;

    // reset:
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
    void deserialise(const nlohmann::json& j);

    // visualise to 'VisualMinesweeperCell' method
    VisualMinesweeperCell visualise() const;

  private:
    // +----------------------+
    // | static const fields: |
    // +----------------------+

    // serialisation keys:
    static const char* CELL_VERSION_KEY;
    // basic serialisation structure keys:
    static const char* IS_VISIBLE_KEY;
    static const char* HAS_MINE_KEY;
    static const char* IS_MARKED_KEY;
    static const char* NUM_OF_MINES_AROUND_KEY;

    // +------------------+
    // | private methods: |
    // +------------------+

    nlohmann::json serialise_() const;

    void deserialise_(const nlohmann::json& j);
};

} // namespace minesweeper

#endif // MINESWEEPER_CELL_H