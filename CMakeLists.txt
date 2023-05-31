cmake_minimum_required(VERSION 3.21)
project(RockPaperScissors)

set(CMAKE_CXX_STANDARD 17)

add_executable(RockPaperScissors main.cpp Grid.cpp Resolution.cpp)
target_link_libraries(RockPaperScissors ncurses)
