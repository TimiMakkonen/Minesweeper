#ifndef MINESWEEPER_VISUAL_MINESWEEPER_CELL_H
#define MINESWEEPER_VISUAL_MINESWEEPER_CELL_H

namespace minesweeper {

enum class VisualMinesweeperCell {
    UNCHECKED = -1,
    EMPTY,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    MINE, // not a typo! :D
    MARKED
};

} // namespace minesweeper

#endif // MINESWEEPER_VISUAL_MINESWEEPER_CELL_H