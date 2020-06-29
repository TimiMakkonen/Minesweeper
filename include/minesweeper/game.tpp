#ifndef MINESWEEPER_GAME_H
#error __FILE__ should only be included from game.h
#endif

#include <minesweeper/game.h>
#include <minesweeper/visual_minesweeper_cell.h>

namespace minesweeper {

template <class Container, typename std::enable_if<is_expandable_1D_sequence_container<Container>::value, int>::type>
Container Game::visualise() const {

    Container output;
    using output_cell_data_type = typename Container::value_type;

    for (int y = 0; y < this->gridHeight; ++y) {
        for (int x = 0; x < this->gridWidth; ++x) {
            
            output.push_back(this->visualiseCell_<output_cell_data_type>(x, y));
        }
    }

    return output;
}

template <class Container, typename std::enable_if<is_expandable_2D_sequence_container<Container>::value, int>::type>
Container Game::visualise() const {

    Container output;
    using output_cell_data_type = typename Container::value_type::value_type;

    for (int y = 0; y < this->gridHeight; ++y) {
        typename Container::value_type outputRow;
        for (int x = 0; x < this->gridWidth; ++x) {

            outputRow.push_back(this->visualiseCell_<output_cell_data_type>(x, y));
        }

        output.push_back(outputRow);
    }

    return output;
}

template <class T>
T Game::visualiseCell(const int X, const int Y) const {

    return this->visualiseCell_<T>(X, Y);
}

template <class T>
T Game::visualiseCell_(const int X, const int Y) const {

    T output;

    VisualMinesweeperCell cellVisual;
    if (this->isCellMarked(X, Y)) {
        cellVisual = VisualMinesweeperCell::MARKED;
    } else if (!this->isCellVisible(X, Y)) {
        cellVisual = VisualMinesweeperCell::UNCHECKED;
    } else {
        if (this->doesCellHaveMine(X, Y)) {
            cellVisual = VisualMinesweeperCell::MINE;
        } else {
            cellVisual = static_cast<VisualMinesweeperCell>(this->numOfMinesAroundCell(X, Y));
        }
    }

    output = static_cast<T>(cellVisual);

    return output;
}

} // namespace minesweeper
