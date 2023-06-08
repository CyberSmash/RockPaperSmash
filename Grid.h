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

#include "Unit.h"
#include "Direction.h"

class Grid {
public:
    Grid(int rows, int cols);

    /**
     * @brief Process a single turn.
     *
     * This calls all necessary functions to update the state of the grid, making it ready
     * to be drawn.
     */
    void process_turn();

    /**
     * @brief Process all of the moves.
     *
     * This will perform any actual moves. This is done after preprocessing the moves.
     */
    void process_moves();

    /**
     * @brief Clear the grid.
     *
     * Sets all squares to UnitType::None.
     *
     * @param grid The grid to be cleared.
     */
    void clear_grid(vector<vector<Unit>>* grid);

    /**
     * @brief Determine the winner of an encounter.
     *
     * @param unit_a - The first unit's type
     * @param unit_b - The second unit's type.
     * @return Which type is the winner of the encounter.
     */
    UnitType winner(UnitType unit_a, UnitType unit_b);

    /**
     * @brief Move a unit.
     *
     * @param row The row of the unit
     * @param col The column of the unit.
     * @param direction The direction in which the unit should be moved.
     */
    void move_unit(int row, int col, Direction direction);

    /**
     * @brief Get a Unit type by row and column for the current grid.
     * @param row The row in question
     * @param col The column in question
     * @return A unit type of the unit at the given row and column.
     */
    UnitType get_unit_type(int row, int col);

    /**
     * Add a unit to the grid.
     *
     * N.B. This will erase any existing unit at the provided location.
     *
     * @todo Consider changing row/col to Point.
     *
     * @param row The row to add the unit.
     * @param col The column to add the unit.
     * @param type The type of unit.
     */
    void add_unit(int row, int col, UnitType type);

    /**
     * @brief Returns a pointer to the grid currently being rendered to the screen.
     *
     * @return The grid that will be printed to the screen in the next frame.
     */
    vector<vector<Unit>>* get_current_grid();

    /**
     * @brief Returns a pointer to the grid that will be rendered to the screen next frame.
     * @return The grid that will be printed to the screen in the next frame.
     */
    vector<vector<Unit>>* get_next_grid();

    /**
     * @brief Get the character representation of a unit at a given row and column.
     *
     * @todo: Consider changing row/col to Point.
     *
     * @param row The row of the unit.
     * @param col The column of the unit.
     * @return The character that should be drawn.
     */
    char get_char(int row, int col);
    Point direction_to_row_col(Direction direction, Point current_loc);
private:

    /**
     * @brief Process all of the resolved conflicts.
     *
     * This is used when we have a "KABOOM" unit, and we need to switch out the kaboom
     * Unit type for the actual winner of the encounter.
     */
    void process_resolutions();

    /**
     * @brief Preprocess all of the moves before actually making them.
     *
     * This allows additional processing to be done based on the knowledge of where a unit is,
     * and where it is going. For example, if unit A is moving to to point B, currently occupied by
     * Unit B and Unit B is moving to point A, they can be disallowed from "moving through" each other.
     */
    void preprocess_moves();


    vector<Point> get_neighbors(int row, int col);

    vector<vector<Unit>> grid_a;
    vector<vector<Unit>> grid_b;
    vector<vector<Unit>>* current_grid;
    vector<vector<Unit>>* next_grid;
    vector<vector<Unit>> resolutions;

};


#endif //ROCKPAPERSCISSORS_GRID_H
