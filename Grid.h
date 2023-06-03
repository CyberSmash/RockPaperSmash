//
// Created by Jordan Sebastian on 5/20/23.
//

#ifndef ROCKPAPERSCISSORS_GRID_H
#define ROCKPAPERSCISSORS_GRID_H
#include <memory>
using std::unique_ptr;

#include <list>

#include <vector>
using std::vector;

#include "UnitType.h"
#include "Resolution.h"
enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE,
    MAX,
};



class Grid {
public:
    Grid(int rows, int cols);
    void process_turn();
    void process_moves();
    void clear_grid(vector<vector<UnitType>>* grid);
    UnitType winner(UnitType unit_a, UnitType unit_b);
    void move_unit(int row, int col, Direction direction);
    UnitType get_unit_type(int row, int col);
    void add_unit(int row, int col, UnitType type);
    vector<vector<UnitType>>* get_current_grid();
    vector<vector<UnitType>>* get_next_grid();

    char get_string(int row, int col);

private:


    void process_resolutions();
    vector<vector<UnitType>> grid_a;
    vector<vector<UnitType>> grid_b;
    vector<vector<UnitType>>* current_grid;
    vector<vector<UnitType>>* next_grid;
    vector<vector<UnitType>> resolutions;

};


#endif //ROCKPAPERSCISSORS_GRID_H
