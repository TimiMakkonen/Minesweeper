
set(EXAMPLE_SOURCE_LIST
	minesweeper_examples.cpp
)

set(EXAMPLE_APP_NAME "MinesweeperExamples")
add_executable("${EXAMPLE_APP_NAME}" ${EXAMPLE_SOURCE_LIST})

target_link_libraries("${EXAMPLE_APP_NAME}" PRIVATE "${LIBRARY_NAME}")

# If CLANG_TIDY_EXE found and enabled, use it
if(CLANG_TIDY_EXE AND CLAND_TIDY_EXAMPLES)
    set_target_properties("${EXAMPLE_APP_NAME}" PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}" )
endif()

