
set(EXAMPLE_SOURCE_LIST
	Minesweeper_Examples.cpp
)

set(EXAMPLE_APP_NAME "MinesweeperExamples")
add_executable("${EXAMPLE_APP_NAME}" ${EXAMPLE_SOURCE_LIST})

target_link_libraries("${EXAMPLE_APP_NAME}" PRIVATE "${LIBRARY_NAME}")

