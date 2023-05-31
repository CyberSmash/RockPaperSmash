//
// Created by Jordan Sebastian on 5/20/23.
//

#ifndef ROCKPAPERSCISSORS_GRID_H
#define ROCKPAPERSCISSORS_GRID_H
#include <memory>
using std::unique_ptr;

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

auto for_all = [](auto& grid, auto func) {
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            func(i, j, grid);
        }
    }
};

class Grid {
public:
    Grid(int rows, int cols);
    void process_turn();
    void process_moves();
    void clear_grid(vector<vector<UnitType>>* grid);
    UnitType winner(UnitType unit_a, UnitType unit_b);
    void move_unit(int row, int col, Direction direction);
    void add_unit(int row, int col, UnitType type);
    vector<vector<UnitType>>* get_current_grid();
    vector<vector<UnitType>>* get_next_grid();

    char get_string(int row, int col);

private:

    int find_resolution(int row, int col);
    void process_resolutions();
    vector<vector<UnitType>> grid_a;
    vector<vector<UnitType>> grid_b;
    vector<vector<UnitType>>* current_grid;
    vector<vector<UnitType>>* next_grid;
    vector<Resolution> resolutions;


};


#endif //ROCKPAPERSCISSORS_GRID_H
