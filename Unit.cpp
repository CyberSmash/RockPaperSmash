//
// Created by chrisnoper on 6/4/23.
//
#include <stdlib.h>
#include "Unit.h"
#include "UnitType.h"


Unit::Unit() : type(UnitType::NONE), next_direction(Direction::NONE) {
}

Unit::Unit(UnitType type) : type(type), next_direction(Direction::NONE) {

}


UnitType Unit::get_type() {
    return type;
}


void Unit::set_type(UnitType new_type) {
    type = new_type;
}

void Unit::set_next_move(Point next) {
    next_move = next;
}

Direction Unit::calc_next_move(Grid& grid, int row, int col)
{
    (void)grid;
    (void)row;
    (void)col;
    switch(get_type())
    {
        case UnitType::ROCK:
            return Direction::RIGHT;
        case UnitType::SCISSORS:
            return Direction::LEFT;
        case UnitType::PAPER:
            return static_cast<Direction>(rand() % Direction::NONE);
        case UnitType::NONE:
            [[fallthrough]];
        case UnitType::KABOOM:
            return Direction::NONE;

    }

    // Probably impossible but satisfies the compiler.
    return Direction::NONE;
}

void Unit::set_next_direction(Direction direction) {
    next_direction = direction;
}

Direction Unit::get_next_direction() {
    return next_direction;
}

Point Unit::get_next_move() {
    return next_move;
}
