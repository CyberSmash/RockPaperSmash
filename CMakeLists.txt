cmake_minimum_required(VERSION 3.21)
project(RockPaperScissors)

set(CMAKE_CXX_STANDARD 17)

add_executable(RockPaperScissors main.cpp Grid.cpp Resolution.cpp Unit.cpp Unit.h Point.cpp Point.h Direction.h)
target_compile_options(RockPaperScissors PRIVATE -Wall -Wextra -pedantic -Werror)
target_link_libraries(RockPaperScissors ncurses)
