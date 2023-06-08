//
// Created by chrisnoper on 6/4/23.
//

#ifndef ROCKPAPERSCISSORS_UNIT_H
#define ROCKPAPERSCISSORS_UNIT_H
#include "UnitType.h"
#include "Point.h"
#include "Direction.h"

class Grid;

class Unit {

public:
    /**
     * @brief Construct a new Unit object. Defaults to UnitType::None.
     */
    Unit();

    /**
     * @brief Construct a new Unit object
     * @param type The unit type to use.
     */
    explicit Unit(UnitType type);

    /**
     * @brief Get the type of the unit.
     * @return The unit type.
     */
    UnitType get_type();

    /**
     * @brief Set the type of the unit.
     * @param new_type - The new type to set.
     */
    void set_type(UnitType new_type);

    /**
     * @brief Set the next coordinate that this unit will move to.
     *
     * @param next The row/col of the next move.
     */
    void set_next_move(Point next);


    Point get_next_move();

    /**
     * @brief Set the next direction this unit will travel in.
     *
     * @param direction The direction to travel.
     */
    void set_next_direction(Direction direction);

    /**
     * @brief Gets the next direciton this unit will move.
     *
     * @return The direction this unit will move.
     */
    Direction get_next_direction();

    /**
     * @brief Calculate the next move for this unit.
     *
     * @todo: Is it more clumsy to need to pass the row and column of a unit to this function,
     * OR for the unit to store this information, needing to update it, when its somewhat redundant.
     *
     * @param grid The grid to use.
     * @param row The row of this unit.
     * @param col The column of this unit.
     *
     * @return The next direction.
     */
    Direction calc_next_move(Grid& grid, int row, int col);

private:

    UnitType type;
    Point next_move;
    Direction next_direction;
};


#endif //ROCKPAPERSCISSORS_UNIT_H
