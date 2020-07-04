#ifndef MINESWEEPER_GAME_H
#error __FILE__ should only be included from game.h
#endif

#include <minesweeper/game.h>
#include <minesweeper/visual_minesweeper_cell.h>

namespace minesweeper {

template <class Container, typename std::enable_if<is_expandable_1d_sequence_container<Container>::value, int>::type>
Container Game::visualise() const {

    Container output;
    using output_cell_data_type = typename Container::value_type;

    for (int y = 0; y < this->_gridHeight; ++y) {
        for (int x = 0; x < this->_gridWidth; ++x) {

            output.push_back(static_cast<output_cell_data_type>(this->visualiseCell_(x, y)));
        }
    }

    return output;
}

template <class Container, typename std::enable_if<is_expandable_2d_sequence_container<Container>::value, int>::type>
Container Game::visualise() const {

    Container output;
    using output_cell_data_type = typename Container::value_type::value_type;

    for (int y = 0; y < this->_gridHeight; ++y) {
        typename Container::value_type outputRow;
        for (int x = 0; x < this->_gridWidth; ++x) {

            outputRow.push_back(static_cast<output_cell_data_type>(this->visualiseCell_(x, y)));
        }

        output.push_back(outputRow);
    }

    return output;
}

template <class T>
T Game::visualiseCell(const int X, const int Y) const {

    if (X < 0 || Y < 0 || X >= this->_gridWidth || Y >= this->_gridHeight) {
        throw std::out_of_range(
            "Game::visualiseCell(const int X, const int Y): Trying to visualise a cell outside the grid.");
    }

    return static_cast<T>(this->visualiseCell_(X, Y));
}

} // namespace minesweeper
