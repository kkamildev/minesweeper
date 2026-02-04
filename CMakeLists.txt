
cmake_minimum_required(VERSION 3.16)

project(minesweeper CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(raylib)

add_executable(minesweeper src/main.cpp)

target_link_libraries(minesweeper raylib)

if(UNIX AND NOT APPLE)
    target_link_libraries(minesweeper m pthread dl rt X11)
endif()