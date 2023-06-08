//
// Created by chrisnoper on 6/4/23.
//

#include "Point.h"

Point::Point() : row(0), col(0) {

}

/**
 * @brief Construct a new Point object
 * @param row - The row.
 * @param col - The column.
 */
Point::Point(int row, int col) : row(row), col(col) {

}

bool Point::operator==(const Point &other) const {
    return row == other.row && col == other.col;
}
