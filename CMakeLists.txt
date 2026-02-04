
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

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

if(UNIX AND NOT APPLE)
    target_link_libraries(minesweeper m pthread dl rt X11)
endif()