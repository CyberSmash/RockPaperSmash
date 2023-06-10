//
// Created by Jordan Sebastian on 5/20/23.
//

#include "Grid.h"
#include <ncurses.h>
#include <stdlib.h>
#include <cassert>

auto for_all = [](auto& grid, auto func) {
    for (unsigned int i = 0; i < grid.size(); i++)
    {
        for (unsigned int j = 0; j < grid[i].size(); j++)
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

    for (unsigned int i = 0; i < grid_a.size(); i++)
    {
        for (unsigned int j = 0; j < grid_a[i].size(); j++)
        {
            grid_a[i][j].set_type( UnitType::NONE);
            grid_b[i][j].set_type( UnitType::NONE);
            resolutions[i][j].set_type( UnitType::NONE); // Not strictly speaking necessary.
        }
    }
}

Point Grid::direction_to_row_col(Direction direction, Point current_loc)
{
    Point new_loc;
    auto& cg = *current_grid;
    switch (direction)
    {
        case UP:
            if (current_loc.row == 0)
            {
                new_loc.row = cg.size() - 1;
            }
            else
            {
                new_loc.row = current_loc.row - 1;
            }
            new_loc.col = current_loc.col;
            break;
        case DOWN:
            new_loc.row = (current_loc.row + 1) % cg.size();
            new_loc.col = current_loc.col;
            break;
        case LEFT:
            if (current_loc.col == 0)
            {
                new_loc.col = cg[current_loc.row].size() - 1;
            }
            else
            {
                new_loc.col = current_loc.col - 1;
            }
            new_loc.row = current_loc.row;
            break;
        case RIGHT:
            new_loc.col = (current_loc.col + 1) % cg[current_loc.row].size();
            new_loc.row = current_loc.row;
            break;
        default:
            return {-1, -1};
    }
    return new_loc;
}

void Grid::move_unit(int row, int col, Direction direction) {
    int new_row = 0;
    int new_col = 0;

    auto &ng = *next_grid;
    auto &cg = *current_grid;

    Point new_loc = direction_to_row_col(direction, Point(row, col));

    if (ng[new_row][new_col].get_type() == UnitType::NONE)
    {
        ng[new_loc.row][new_loc.col] = cg[row][col];
    }
    else
    {
        ng[row][col].set_type(UnitType::KABOOM);
        resolutions[row][col].set_type(winner(cg[row][col].get_type(), ng[row][col].get_type()));
    }

}

void Grid::clear_grid(vector<vector<Unit>> *grid) {

    for_all(*grid, [](int row, int col, vector<vector<Unit>> &grid) {
        grid[row][col].set_type(UnitType::NONE);
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
    (*current_grid)[row][col].set_type(type);
}

void Grid::process_resolutions()
{
    auto& cg = *current_grid;

    for_all(cg, [this](int row, int col, vector<vector<Unit>> &grid) {
        if (grid[row][col].get_type() == UnitType::KABOOM)
        {
            grid[row][col].set_type(resolutions[row][col].get_type());
        }
    });
}

void Grid::preprocess_moves()
{
    auto& cg = *current_grid;

    for_all(cg, [this](int row, int col, vector<vector<Unit>> &grid) {
        UnitType current_type = grid[row][col].get_type();
        if (current_type == UnitType::NONE || current_type == UnitType::KABOOM)
            return;

        Direction direction = grid[row][col].calc_next_move(*this, row, col);
        Point new_loc = direction_to_row_col(direction, Point(row, col));
        grid[row][col].set_next_move(new_loc);
        grid[row][col].set_next_direction(direction);

    });
}

vector<Point> Grid::get_neighbors(int row, int col)
{
    vector<Point> points;
    if (row == 0)
    {
        points.emplace_back(next_grid->size() - 1, col);
        points.emplace_back(row + 1, col);
    }
    else if (row == static_cast<int>(next_grid->size() - 1))
    {
        points.emplace_back(0, col);
        points.emplace_back(row - 1, col);
    }
    else
    {
        points.emplace_back(row - 1, col);
        points.emplace_back(row + 1, col);
    }

    if (col == 0)
    {
        points.emplace_back(row, next_grid->at(row).size() - 1);
        points.emplace_back(row, col + 1);
    }
    else if (col == static_cast<int>(next_grid->at(row).size() - 1))
    {
        points.emplace_back(row, 0);
        points.emplace_back(row, col - 1);
    }
    else
    {
        points.emplace_back(row, col - 1);
        points.emplace_back(row, col + 1);
    }

    return points;

}

void Grid::process_moves()
{
    auto& cg = *current_grid;
    auto& ng = *next_grid;

    for_all(cg, [this, ng](int row, int col, vector<vector<Unit>> &grid) {
        Point current_point(row, col);
        UnitType current_type = grid[row][col].get_type();
        if (current_type == UnitType::NONE || current_type == UnitType::KABOOM)
            return;


        Point next_point = grid[row][col].get_next_move();
        vector<Point> neighbors = get_neighbors(row, col);

        if (grid[next_point.row][next_point.col].get_type() != UnitType::NONE
        && grid[next_point.row][next_point.col].get_next_move() == current_point)
        {
            // Determine who the winner would be
            UnitType win_result = winner(grid[next_point.row][next_point.col].get_type(), current_type);
            if (win_result == current_type)
            {
                // We won, so update the resolution, but add a kaboom at our next location
                resolutions[next_point.row][next_point.col].set_type(current_type);
                (*next_grid)[next_point.row][next_point.col].set_type(UnitType::KABOOM);
            }
            else
            {
                // We lost, so update the resolution, but add a kaboom at our current location
                resolutions[row][col].set_type(win_result);
                (*next_grid)[row][col].set_type(UnitType::KABOOM);
            }
            grid[next_point.row][next_point.col].set_type(UnitType::NONE);
            grid[row][col].set_type(UnitType::NONE);
        }

        else if ((*next_grid)[next_point.row][next_point.col].get_type() != UnitType::NONE)
        {
            UnitType the_winner = winner(grid[row][col].get_type(), (*next_grid)[next_point.row][next_point.col].get_type());
            resolutions[next_point.row][next_point.col].set_type(the_winner);
            (*next_grid)[next_point.row][next_point.col].set_type(UnitType::KABOOM);
        }
        else
        {
            (*next_grid)[next_point.row][next_point.col].set_type(grid[row][col].get_type());
        }


    });
}

void Grid::process_turn() {

    clear_grid(next_grid);
    process_resolutions();
    preprocess_moves();
    process_moves();

    // Switch our grids.
    current_grid = (current_grid == &grid_a) ? &grid_b : &grid_a;
    next_grid = (next_grid == &grid_a) ? &grid_b : &grid_a;
}

vector<vector<Unit>> *Grid::get_current_grid() {
    return current_grid;
}

vector<vector<Unit>>* Grid::get_next_grid() {
    return next_grid;
}

char Grid::get_char(int row, int col)
{
    UnitType type = current_grid->at(row).at(col).get_type();
    return RepresentationMatrix[static_cast<int>(type)];
}

UnitType Grid::get_unit_type(int row, int col)
{
    return current_grid->at(row).at(col).get_type();
}