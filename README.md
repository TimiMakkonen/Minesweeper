# Minesweeper game library

[![GitHub License](https://img.shields.io/github/license/TimiMakkonen/minesweeper)](https://github.com/TimiMakkonen/minesweeper/LICENSE.MIT)
![GitHub Latest Release Tag](https://img.shields.io/github/v/tag/TimiMakkonen/minesweeper)

C++ library containing the core logic of Minesweeper game.

## Installation

If you want to clone this git repository, use

```console
git clone --recurse-submodules https://github.com/TimiMakkonen/minesweeper.git
```

or something similar to ensure that ['ArthurSonzogni/nlohmann_json_cmake_fetchcontent'](https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent) submodule gets cloned properly.

Then you can use this library as part of your own CMake project as you desire.

## How to use

Minimum C++ version required: C++11.

To use this Minesweeper game library, pass your implementation of provided 'IRandom' pure virtual class to 'Game' class or alternatively pass it as an default option to the 'Game' class statically, so that it can be used by all the instances of 'Game' class.

If you are truly lazy, and/or you do not need to have/control randomness elsewhere, you can simply initialise and pass 'Random' (provided implementation of 'IRandom' that uses 'std::mt19937') as instructed above.

You can check 'examples' functions in ['minesweeper_examples.cpp'](https://github.com/TimiMakkonen/minesweeper/examples/minesweeper_examples.cpp) for more detailed usage explanation and ['game.h'](https://github.com/TimiMakkonen/minesweeper/include/minesweeper/game.h) for the main interface.

## Version History

### Version 8.3.0 (Develop)

* Added reset and newGame methods.
  * 'reset(bool keepCreatedMines = false)' method keeps current grid size and number of mines parameters and allows to keep currently set mines as well if argument 'keepCreatedMines' is set to true.
  * 'newGame' methods start completely new game with different grid sizes and number of mines.
* Formatted code using ['clang-format'](https://clang.llvm.org/docs/ClangFormat.html).
* Added virtual destructor to 'IRandom'.
* Tidied code using ['clang-tidy'](https://clang.llvm.org/extra/clang-tidy/).

### Version 8.2.0

* Added serialisation and deserialisation methods.
  * Using ['nlohmann/json'](https://github.com/nlohmann/json) header-only library to do this in 'json'-form.
  * Added ['ArthurSonzogni/nlohmann_json_cmake_fetchcontent'](https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent) as submodule to this project to implement this neatly in lightweight form.
  * Already considering about using different (faster) 'JSON' library or different serialisation data
    format since this current method is relatively slow for larger grids.
* Added some tests.
* Slightly edited 'README.MD'.

### Version 8.1.0

* Made public 'createMinesAndNums(const int initChosenX, const int initChosenY)' redundant.
  * Can still be called as before, but completely unnecessary (and not recommended) to do this manually from now on.
* 'markInputCoordinates(const int X, const int Y)' now throws if 'checkInputCoordinates(const int X, const int Y)'
  or 'createMinesAndNums(const int initChosenX, const int initChosenY)' has not been called before it.
* Added getters for grid height, width and number of mines.
* Added some tests.
* Added verifications to some methods.
* Updated ['minesweeper_examples.cpp'](https://github.com/TimiMakkonen/minesweeper/examples/minesweeper_examples.cpp).
* Slightly edited 'README.MD'.

### Version 8.0.0

* Renamed the 'Grid' class to 'Game'.
  * Might cause historical inconsistencies and confusion, since the original console game version had a class with this name. (v6.1.1 and lower)
  * Should increase clarity for further releases of this library.
* Changed file naming convention from 'PascalCase' to 'snake_case'.
* Changed namespace naming convention from 'PascalCase' to 'lowercase'.
* Added missing '#include\<vector\>' to 'i_random.h'.
* Slightly edited 'README.MD'.

### Version 7.0.0

* Separated original Minesweeper game (on console) into:
  * 'minesweeper' (game library) and
  * ['console-minesweeper'](https://github.com/TimiMakkonen/console-minesweeper) (console version of the game which uses the aforementioned library).
* Turned VS solution into CMake library.
  * Added '(google)testing'. (Not implemented yet.)
  * Added 'examples' executable to allow showcasing library.
    * Added some usage instructions and examples.
* New classes 'IRandom' and 'Random':
  * Separating randomising part of creating mines from rest of the 'Grid'.
  * IRandom is an abstract class which provides 'Grid' with ability to shuffle a vector of ints.
  * You can create your own class which implements 'IRandom' or use provided 'Random' class for simplicity.
  * You can pass 'IRandom' via constructor or set 'defaultRandom' static property.
    (Class field set by constructor has the priority.)
* Slightly improved readability of code.

## Pre Separaration Version History

### Version 6.1.1

* Added validity check for the number of mines in the constructor of 'Grid'.
* Created static methods in 'Grid' class to tell the maximum and minimum number of mines allowed in a grid of given size.

### Version 6.1.0

* Separated 'gridSize' into 'gridHeight' and 'gridWidth' to allow 'non-square' grids both in 'Grid' and 'Game'.

### Version 6.0

* Moved print functionality from 'Grid' to 'Game'.
* Changed cerr's into asserts and throws.
* Moved and changed some of the game logic.

### Version 5.2

* Slightly improved readability of the code and fixed minor things.
* Changed minimum size of grid into 4x4 to stop softlock when choosing number of mines for smaller grids.

### Version 5.1

* Added win condition (all non mines visible/checked).
* Made impossible to mark a cell on the first turn.
* Slightly improved readability of the code.

### Version 5.0

* Updated program from C-style to OOP.
  * New 'Game', 'Grid' and 'Cell' classes.
* Improved readability and efficiency of some parts of code.
  * Changed variable naming convention from pascal to camel and made names easier to understand.
* Changed pseudo-random number generator from 'rand' to 'std::mt19937' (Mersenne Twister 19937) to improve randomness.

### Version 4.0

* Continued with improvements on the input system.
* Increased maximum size of the grid up to 99x99. (I do not recommend grids larger than 50x50.)
* Program now shows your last grid spot input.
* Made function 'TryInputToCoordinates' mostly future proof.
  * can handle grid sizes up to 675 (overkill tbh)
  * able to give error messages and decide when new input is needed.
* Improved games structure for all the different possibilities.
* Added and made HELP menu optional. (show up in the beginning and later upon request)
* Improved instructions and other text output.
* Added ability to play again.
* Made sure that you cannot lose on the first turn.
  * Creation of the grid has been moved to take into account players first choice, making sure player will make some progress.

### Version 3.0

* Slightly improved readability of the code.
* Started to deal with proper handling of wrong inputs.
* Added initial win screen and win condition, if you manage to mark all mines.
* Ability to unmark grid spots by marking them again (in case you made a mistake).
* Initial options/special input menu implemented.

### Version 2.0

* Fixed some visual mistakes and made the game look prettier.
* Added automatic mine check for squares around, when choosing a grid location with no surrounding mines.
* Fixes input for 2 digit numbers.
* Initial losing screen.
* More convenient input system.
* \+ Background work to make next additions/fixes easier to implement.

### Version 1.0

Initial version of the game. Mostly in working condition.

## Fixes and features left to consider/implement

* Add an option to add mines by percentage, not by quantity.
* Add method which returns a visual representation of the current state of the grid in short string form.
* Add more serialisation options.
* Add verifications to deserialisation methods to check validity of the given game.
