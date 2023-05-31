//
// Created by Jordan Sebastian on 5/20/23.
//

#include "Grid.h"
#include <ncurses.h>

const UnitType resolutionMatrix[3][3] = {
        {UnitType::NONE, UnitType::PAPER, UnitType::ROCK},
        {UnitType::PAPER, UnitType::NONE, UnitType::SCISSORS},
        {UnitType::ROCK, UnitType::SCISSORS, UnitType::NONE}
}; // 0 = rock, 1 = paper, 2 = scissors

const char RepresentationMatrix[5] = {'R', 'P', 'S', ' ', '#'};

Grid::Grid(int rows, int cols): current_grid(&grid_a), next_grid(&grid_b)
{
    grid_a.resize(rows);
    grid_b.resize(rows);
    for (int i = 0; i < rows; i++)
    {
        grid_a[i].resize(cols);
        grid_b[i].resize(cols);
    }

    for (int i = 0; i < grid_a.size(); i++)
    {
        for (int j = 0; j < grid_a[i].size(); j++)
        {
            grid_a[i][j] = UnitType::NONE;
            grid_b[i][j] = UnitType::NONE;
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
                new_col = current_grid->size() - 1;
            }
            else
            {
                new_col = col - 1;
            }
            new_row = row;
            break;
        case RIGHT:
            if (col == current_grid->size() - 1)
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
        resolutions.emplace_back(row, col, winner(cg[row][col], ng[row][col]));
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

    return resolutionMatrix[static_cast<int>(unit_a)][static_cast<int>(unit_b)];
}

void Grid::add_unit(int row, int col, UnitType type) {
    (*current_grid)[row][col] = type;
}

int Grid::find_resolution(int row, int col)
{

    for (int idx = 0; idx < resolutions.size(); idx++)
    {
        if (resolutions[idx].row == row && resolutions[idx].col == col)
        {
            return idx;
        }
    }

    return -1;
}

void Grid::process_resolutions()
{
    int row, col;
    //auto& cg = *current_grid;
    auto& ng = *next_grid;

    for_all(ng, [this](int row, int col, vector<vector<UnitType>> &grid) {
        if (grid[row][col] != UnitType::KABOOM)
            return;

        int resolution_idx = find_resolution(row, col);
        if (resolution_idx == -1)
            return;
        grid[row][col] = resolutions[resolution_idx].type;
        resolutions.erase(std::next(resolutions.begin(),resolution_idx));
    });
}

void Grid::process_moves()
{
    auto& cg = *current_grid;
    auto& ng = *next_grid;

    for_all(cg, [this](int row, int col, vector<vector<UnitType>> &grid) {
        if (grid[row][col] == UnitType::NONE || grid[row][col] == UnitType::KABOOM)
            return;

        auto direction = static_cast<Direction>(rand() % Direction::NONE);
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
