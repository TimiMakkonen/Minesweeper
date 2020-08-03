#ifndef MINESWEEPER_GAME_H
#error __FILE__ should only be included from game.h
#endif

#include <stdexcept>   // std::out_of_range
#include <type_traits> // std::enable_if

#include <minesweeper/game.h>
#include <minesweeper/visual_minesweeper_cell.h>

namespace minesweeper {

template <class T>
T Game::visualiseCell(const int x, const int y) const {

    if (x < 0 || y < 0 || x >= this->_gridWidth || y >= this->_gridHeight) {
        throw std::out_of_range(
            "Game::visualiseCell(const int X, const int Y): Trying to visualise a cell outside the grid.");
    }

    return static_cast<T>(this->visualiseCell_(x, y));
}

template <class Container, typename std::enable_if<is_expandable_1d_sequence_container<Container>::value, int>::type>
Container Game::visualise() const {
    return this->visualisationDetailIn1d_<Container>(&Game::visualiseCell_);
}

template <class Container, typename std::enable_if<is_expandable_2d_sequence_container<Container>::value, int>::type>
Container Game::visualise() const {
    return this->visualisationDetailIn2d_<Container>(&Game::visualiseCell_);
}

template <class T>
T Game::visualiseCellSolution(const int x, const int y) const {

    if (x < 0 || y < 0 || x >= this->_gridWidth || y >= this->_gridHeight) {
        throw std::out_of_range("Game::visualiseCellSolution(const int X, const int Y): "
                                "Trying to visualise solution of a cell outside the grid.");
    }

    return static_cast<T>(this->visualiseCellSolution_(x, y));
}

template <class Container, typename std::enable_if<is_expandable_1d_sequence_container<Container>::value, int>::type>
Container Game::visualiseSolution() const {
    return this->visualisationDetailIn1d_<Container>(&Game::visualiseCellSolution_);
}

template <class Container, typename std::enable_if<is_expandable_2d_sequence_container<Container>::value, int>::type>
Container Game::visualiseSolution() const {
    return this->visualisationDetailIn2d_<Container>(&Game::visualiseCellSolution_);
}

template <class T>
T Game::visualiseCellSolutionData(const int x, const int y) const {

    if (x < 0 || y < 0 || x >= this->_gridWidth || y >= this->_gridHeight) {
        throw std::out_of_range("Game::visualiseCellSolutionData(const int X, const int Y): "
                                "Trying to visualise solution data of a cell outside the grid.");
    }

    return static_cast<T>(this->visualiseCellSolutionData_(x, y));
}

template <class Container, typename std::enable_if<is_expandable_1d_sequence_container<Container>::value, int>::type>
Container Game::visualiseSolutionData() const {
    return this->visualisationDetailIn1d_<Container>(&Game::visualiseCellSolutionData_);
}

template <class Container, typename std::enable_if<is_expandable_2d_sequence_container<Container>::value, int>::type>
Container Game::visualiseSolutionData() const {
    return this->visualisationDetailIn2d_<Container>(&Game::visualiseCellSolutionData_);
}

template <class Container>
Container Game::visualisationDetailIn1d_(VisualMinesweeperCell (Game::*cellVisualisationMethod)(int, int) const) const {

    Container output;
    using output_cell_data_type = typename Container::value_type;

    for (int y = 0; y < this->_gridHeight; ++y) {
        for (int x = 0; x < this->_gridWidth; ++x) {

            output.push_back(static_cast<output_cell_data_type>((this->*cellVisualisationMethod)(x, y)));
        }
    }

    return output;
}

template <class Container>
Container Game::visualisationDetailIn2d_(VisualMinesweeperCell (Game::*cellVisualisationMethod)(int, int) const) const {

    Container output;
    using output_cell_data_type = typename Container::value_type::value_type;

    for (int y = 0; y < this->_gridHeight; ++y) {
        typename Container::value_type outputRow;
        for (int x = 0; x < this->_gridWidth; ++x) {

            outputRow.push_back(static_cast<output_cell_data_type>((this->*cellVisualisationMethod)(x, y)));
        }

        output.push_back(outputRow);
    }

    return output;
}

} // namespace minesweeper
