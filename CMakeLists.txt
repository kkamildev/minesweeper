
cmake_minimum_required(VERSION 3.16)

project(minesweeper CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(raylib)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(
    minesweeper
    src/main.cpp
    ${SOURCES}
    )

target_link_libraries(minesweeper raylib)

add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_CURRENT_BINARY_DIR}/assets
)


if(UNIX AND NOT APPLE)
    target_link_libraries(minesweeper m pthread dl rt X11)
endif()