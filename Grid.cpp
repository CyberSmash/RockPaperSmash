//
// Created by Jordan Sebastian on 5/20/23.
//

#include "Grid.h"
#include <ncurses.h>
#include <stdlib.h>

auto for_all = [](auto& grid, auto func) {
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            func(i, j, grid);
        }
    }
};

const UnitType winner_matrix[3][3] = {
        {UnitType::NONE, UnitType::PAPER, UnitType::ROCK},
        {UnitType::PAPER, UnitType::NONE, UnitType::SCISSORS},
        {UnitType::ROCK, UnitType::SCISSORS, UnitType::NONE}
}; // 0 = rock, 1 = paper, 2 = scissors

const char RepresentationMatrix[5] = {'R', 'P', 'S', ' ', '#'};

Grid::Grid(int rows, int cols): current_grid(&grid_a), next_grid(&grid_b)
{
    grid_a.resize(rows);
    grid_b.resize(rows);
    resolutions.resize(rows);
    for (int i = 0; i < rows; i++)
    {
        grid_a[i].resize(cols);
        grid_b[i].resize(cols);
        resolutions[i].resize(cols);
    }

    for (int i = 0; i < grid_a.size(); i++)
    {
        for (int j = 0; j < grid_a[i].size(); j++)
        {
            grid_a[i][j] = UnitType::NONE;
            grid_b[i][j] = UnitType::NONE;
            resolutions[i][j] = UnitType::NONE; // Not strictly speaking necessary.
        }
    }
}

void Grid::move_unit(int row, int col, Direction direction) {
    int new_row = 0;
    int new_col = 0;

    auto &ng = *next_grid;
    auto &cg = *current_grid;

    switch (direction)
    {
        case UP:
            if (row == 0)
            {
                new_row = current_grid->size() - 1;
            }
            else
            {
                new_row = row - 1;
            }
            new_col = col;
            break;
        case DOWN:
            if (row == current_grid->size() - 1)
            {
                new_row = 0;
            }
            else
            {
                new_row = row + 1;
            }
            new_col = col;
            break;
        case LEFT:
            if (col == 0)
            {
                new_col = current_grid[row].size() - 1;
            }
            else
            {
                new_col = col - 1;
            }
            new_row = row;
            break;
        case RIGHT:
            if (col == current_grid[row].size() - 1)
            {
                new_col = 0;
            }
            else
            {
                new_col = col + 1;
            }
            new_row = row;
            break;
        default:
            return;
    }

    if (ng[new_row][new_col] == UnitType::NONE)
    {
        ng[new_row][new_col] = cg[row][col];
    }
    else
    {
        ng[row][col] = UnitType::KABOOM;
        resolutions[row][col] = winner(cg[row][col], ng[row][col]);
    }

}

void Grid::clear_grid(vector<vector<UnitType>> *grid) {

    for_all(*grid, [](int row, int col, vector<vector<UnitType>> &grid) {
        grid[row][col] = UnitType::NONE;
    });
}

UnitType Grid::winner(UnitType unit_a, UnitType unit_b) {

    if (unit_a == UnitType::KABOOM || unit_b == UnitType::KABOOM)
    {
        return UnitType::KABOOM;
    }
    if (unit_a == UnitType::NONE || unit_b == UnitType::NONE)
    {
        return UnitType::NONE;
    }

    return winner_matrix[static_cast<int>(unit_a)][static_cast<int>(unit_b)];
}

void Grid::add_unit(int row, int col, UnitType type) {
    (*current_grid)[row][col] = type;
}



void Grid::process_resolutions()
{
    int row, col;
    //auto& cg = *current_grid;
    auto& ng = *next_grid;

    for_all(ng, [this](int row, int col, vector<vector<UnitType>> &grid) {
        if (grid[row][col] == UnitType::KABOOM)
        {
            grid[row][col] = resolutions[row][col];
        }
    });
}

void Grid::process_moves()
{
    auto& cg = *current_grid;
    auto& ng = *next_grid;

    for_all(cg, [this](int row, int col, vector<vector<UnitType>> &grid) {
        if (grid[row][col] == UnitType::NONE || grid[row][col] == UnitType::KABOOM)
            return;

        Direction direction; //= static_cast<Direction>(rand() % Direction::NONE);
        switch(grid[row][col])
        {
            case UnitType::ROCK:
                direction = Direction::RIGHT;
                break;

            case UnitType::SCISSORS:
                direction = Direction::LEFT;
                break;

            default:
                direction = Direction::NONE;

        }
        move_unit(row, col, direction);
    });
}

void Grid::process_turn() {
    auto& cg = *current_grid;
    auto& ng = *next_grid;

    clear_grid(next_grid);
    process_resolutions();
    process_moves();

    // Switch our grids.
    current_grid = (current_grid == &grid_a) ? &grid_b : &grid_a;
    next_grid = (next_grid == &grid_a) ? &grid_b : &grid_a;
}

vector<vector<UnitType>> *Grid::get_current_grid() {
    return current_grid;
}

vector<vector<UnitType>>* Grid::get_next_grid() {
    return next_grid;
}

char Grid::get_string(int row, int col)
{
    UnitType type = current_grid->at(row).at(col);
    return RepresentationMatrix[static_cast<int>(type)];
}

UnitType Grid::get_unit_type(int row, int col)
{
    return current_grid->at(row).at(col);
}