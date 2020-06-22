
# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

# Public header files
set(HEADER_FOLDER "${PROJECT_SOURCE_DIR}/include/minesweeper")
set(HEADER_LIST
    "${HEADER_FOLDER}/game.h"
    "${HEADER_FOLDER}/i_random.h"
    "${HEADER_FOLDER}/random.h"
)

# Private header files
set(PRIVATE_HEADER_FOLDER "minesweeper")
set(PRIVATE_HEADER_LIST
    "${PRIVATE_HEADER_FOLDER}/cell.h"
)

# Source files
set(SOURCE_FOLDER "minesweeper")
set(SOURCE_LIST
    "${SOURCE_FOLDER}/cell.cpp"
    "${SOURCE_FOLDER}/game.cpp"
    "${SOURCE_FOLDER}/random.cpp"
)



# Make an automatic library - will be static or dynamic based on user setting
add_library("${LIBRARY_NAME}"
            ${HEADER_LIST}
            ${PRIVATE_HEADER_LIST}
            ${SOURCE_LIST}
)

# We need this directory, and users of our library will need it too
target_include_directories("${LIBRARY_NAME}" PUBLIC "${PROJECT_SOURCE_DIR}/include")

# We need this directory
target_include_directories("${LIBRARY_NAME}" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

# We need this library for json (de)serialisation
target_link_libraries("${LIBRARY_NAME}" PRIVATE nlohmann_json::nlohmann_json)

# All users of this library will need at least C++11
target_compile_features("${LIBRARY_NAME}" PUBLIC cxx_std_11)

# If CLANG_TIDY_EXE found and enabled, use it
if(CLANG_TIDY_EXE AND MINESWEEPER_CLANG_TIDY)
    set_target_properties("${LIBRARY_NAME}" PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}" )
endif()

# IDEs should put the headers in a nice place
# (This might not work as intended)
# source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})